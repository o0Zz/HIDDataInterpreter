#include "HIDReportDescriptor.h"
#include "HIDReportDescriptorElements.h"
#include "HIDReportDescriptorUsages.h"
#include <iostream>
#include <vector>
#include <memory>
#include <stack>
#include <cassert>
#include <algorithm>

//https://github.com/pasztorpisti/hid-report-parser/blob/master/src/hid_report_parser.cpp
//https://docs.kernel.org/hid/hidintro.html

HIDInputOutput::HIDInputOutput(HIDIOType type, uint32_t size, uint32_t id) : 
    type(type), 
    sub_type(0),
    size(size),
    id(id),
    logical_min(0), 
    logical_max(0), 
    physical_min(0), 
    physical_max(0), 
    unit(0),
    unit_exponent(0) 
{

}

HIDInputOutput::~HIDInputOutput()
{

}

HIDInputOutput::HIDInputOutput(const HIDUsage &usage, uint32_t idx) : 
    type(HIDIOType::Unknown),
    sub_type(0),
    size(usage.property.size),
    id(0),
    logical_min(usage.property.logical_min),
    logical_max(usage.property.logical_max),
    physical_min(usage.property.physical_min), 
    physical_max(usage.property.physical_max), 
    unit(usage.property.unit), 
    unit_exponent(usage.property.unit_exponent) 
{           
    if (usage.type == HIDUsageType::GenericDesktop)
    {
        if (usage.sub_type == (uint32_t)HIDUsageGenericDesktopSubType::X)
            this->type = HIDIOType::X;
        else if (usage.sub_type == (uint32_t)HIDUsageGenericDesktopSubType::Y)
            this->type = HIDIOType::Y;
        else if (usage.sub_type == (uint32_t)HIDUsageGenericDesktopSubType::Z)
            this->type = HIDIOType::Z;
        else if (usage.sub_type == (uint32_t)HIDUsageGenericDesktopSubType::Rx)
            this->type = HIDIOType::Rx;
        else if (usage.sub_type == (uint32_t)HIDUsageGenericDesktopSubType::Ry)
            this->type = HIDIOType::Ry;
        else if (usage.sub_type == (uint32_t)HIDUsageGenericDesktopSubType::Rz)
            this->type = HIDIOType::Rz;
        else if (usage.sub_type == (uint32_t)HIDUsageGenericDesktopSubType::HatSwitch)
            this->type = HIDIOType::HatSwitch;
        else if (usage.sub_type == (uint32_t)HIDUsageGenericDesktopSubType::Wheel)
            this->type = HIDIOType::Wheel;
    }
    else if (usage.type == HIDUsageType::Button)
        this->type = HIDIOType::Button;
    else if (usage.type == HIDUsageType::ReportId)
        this->type = HIDIOType::ReportId;
    else if (usage.type == HIDUsageType::Padding)
        this->type = HIDIOType::Padding;
    else if (usage.type == HIDUsageType::VendorDefined)
    {
        this->type = HIDIOType::VendorDefined;
        this->sub_type = usage.sub_type;
    }
    else
    {
        this->type = HIDIOType::Unknown;
        this->sub_type = usage.sub_type;
    }
    this->id = usage.usage_min + idx;
}

/* -------------------------------------------------------------------- */

HIDReportDescriptor::HIDReportDescriptor(const uint8_t *hid_report_data, uint16_t hid_report_data_len)
{
    parse(hid_report_data, hid_report_data_len);
}

/* -------------------------------------------------------------------- */

HIDReportDescriptor::~HIDReportDescriptor()
{

}

/* -------------------------------------------------------------------- */

void HIDReportDescriptor::parse(const uint8_t *hid_report_data, uint16_t hid_report_data_len)
{
    std::vector<HIDElement> hid_report_elements = HIDReportDescriptorElements::parse(hid_report_data, hid_report_data_len);
    std::vector<HIDUsage> hid_report_usage = HIDReportDescriptorUsages::parse(hid_report_elements);

    std::vector<HIDInputOutput> current_data;
    uint32_t current_size = 0;

    for (size_t k = 0; k < hid_report_usage.size(); k++)
    {
        std::vector<HIDIOBlock> *ioblocks = NULL;
        HIDUsage &usage = hid_report_usage[k];

        if (usage.type == HIDUsageType::GenericDesktop)
        {
            if (((HIDUsageGenericDesktopSubType)usage.sub_type) < HIDUsageGenericDesktopSubType::ReportTypeEnd 
                && ((HIDUsageGenericDesktopSubType)usage.sub_type) != HIDUsageGenericDesktopSubType::Pointer)
            {
                m_reports.push_back(std::make_shared<HIDReport>((HIDReportType)usage.sub_type));
            }
        }

        if (m_reports.size() == 0)
            continue;

        if (usage.io_type == HIDUsageIOType::Input)
            ioblocks = &m_reports.back()->inputs;
        else if (usage.io_type == HIDUsageIOType::Output)
            ioblocks = &m_reports.back()->outputs;
        else if (usage.io_type == HIDUsageIOType::Feature)
            ioblocks = &m_reports.back()->features;
        else
            continue;

        for (uint32_t i = 0; i < usage.property.count; i++)
        {
            current_data.push_back(HIDInputOutput(usage, i));
            current_size += usage.property.size;
        }

            //If we read enough data to have a complete byte, we can reorder them and add it to the data
        if (current_size%8 == 0)
        {
            std::vector<HIDInputOutput> ordered_current_data;

            uint32_t data_len = 0;
            for (size_t i = 0; i < current_data.size(); i++)
            {
                ordered_current_data.push_back(current_data[i]);
                data_len += current_data[i].size;

                if (data_len >= 8)
                {
                    if (current_data[i].type == HIDIOType::ReportId || ioblocks->size() == 0)
                        ioblocks->push_back(HIDIOBlock());

                    std::reverse(ordered_current_data.begin(), ordered_current_data.end());
                    ioblocks->back().data.insert(ioblocks->back().data.end(), ordered_current_data.begin(), ordered_current_data.end());
                    ordered_current_data.clear();
                    data_len = 0;
                }
            }

            current_data.clear();
            current_size = 0;
        }
    }

    assert(m_reports.size() > 0);
}
