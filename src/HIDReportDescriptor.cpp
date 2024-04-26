#include "HIDReportDescriptor.h"
#include <iostream>
#include <vector>
#include <memory>
#include <stack>
#include <cassert>

//https://docs.kernel.org/hid/hidreport-parsing.html
#define HID_FUNC_TYPE_MASK 0xFC
#define HID_TYPE_MASK 0x0C
#define HID_LENGTH_MASK 0x03

//https://github.com/pasztorpisti/hid-report-parser/blob/master/src/hid_report_parser.cpp
//https://docs.kernel.org/hid/hidintro.html

//---------------USAGE_PAGE-----------------

#define USAGE_PAGE_GenericDesktop       0x01
#define USAGE_PAGE_Simulation           0x02
#define USAGE_PAGE_VR                   0x03
#define USAGE_PAGE_Sport                0x04
#define USAGE_PAGE_Game                 0x05
#define USAGE_PAGE_GenericDevice        0x06
#define USAGE_PAGE_Keyboard             0x07
#define USAGE_PAGE_LEDs                 0x08
#define USAGE_PAGE_Button               0x09
#define USAGE_PAGE_Ordinal              0x0A
#define USAGE_PAGE_Telephony            0x0B
#define USAGE_PAGE_Consumer             0x0C

//---------------USAGE-----------------

#define USAGE_X             0x30
#define USAGE_Y             0x31
#define USAGE_Z             0x32
#define USAGE_Rx            0x33
#define USAGE_Ry            0x34
#define USAGE_Rz            0x35
#define USAGE_Slider        0x36
#define USAGE_Dial          0x37
#define USAGE_Wheel         0x38
#define USAGE_Hat_switch    0x39

//---------------INPUT-----------------
#define INPUT_Const         0x01
#define INPUT_Var           0x02
#define INPUT_Rel           0x04
#define INPUT_Wrap          0x08
#define INPUT_NLin          0x10
#define INPUT_NPrf          0x20
#define INPUT_Null          0x40
#define INPUT_Vol           0x80

/* -------------------------------------------------------------------- */

typedef enum class HIDCollectionType
{
    HID_COLLECTION_PHYSICAL = 0x00,
    HID_COLLECTION_APPLICATION = 0x01,
    HID_COLLECTION_LOGICAL = 0x02,
    HID_COLLECTION_REPORT = 0x03,
    HID_COLLECTION_NAMED_ARRAY = 0x04,
    HID_COLLECTION_USAGE_SWITCH = 0x05,
    HID_COLLECTION_USAGE_MODIFIER = 0x06
} HIDCollectionType;

class HIDCollection
{
public:
    HIDCollection(HIDCollectionType type) :
        m_type(type)
    {
    }
    ~HIDCollection() {}

    HIDCollectionType m_type;
};

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
    std::vector<std::shared_ptr<HIDItem>> hid_report_items = parseItems(hid_report_data, hid_report_data_len);

    HIDProperty current_property;
    std::shared_ptr<HIDReport> current_report = nullptr;
    std::vector<HIDUsage> current_usages;
    std::shared_ptr<HIDItem> current_usage_page;
    std::stack<std::shared_ptr<HIDCollection>> collection_stack;

    //iterate on hid_report_descriptior
    for (std::shared_ptr<HIDItem> item : hid_report_items)
    {
        if (current_report == nullptr)
            current_report = std::make_shared<HIDReport>(HIDReportType::Unknown);

        switch (item->GetType())
        {
            case HIDItemType::HID_USAGE_PAGE:
            {
                current_usage_page = item;
                break;
            }
            
            case HIDItemType::HID_USAGE:
            {
                if (current_usage_page->GetValueUint32() == USAGE_PAGE_Button)
                {
                    current_usages.push_back(HIDInputType::Button);
                    current_usages.back().usage_min = item->GetValueUint32();
                }
                else if (item->GetValueUint32() < (uint32_t)HIDReportType::MAX)
                {
                    if (current_report->report_type == HIDReportType::Unknown)
                        current_report->report_type = (HIDReportType)item->GetValueUint32();
                }
                else
                {
                    current_usages.push_back(HIDUsage((HIDInputType)item->GetValueUint32()));
                }
                break;
            }

            case HIDItemType::HID_USAGE_MAXIMUM:
            case HIDItemType::HID_USAGE_MINIMUM:
            {
                if (current_usage_page->GetValueUint32() == USAGE_PAGE_Button)
                {
                    if (current_usages.size() == 0)
                        current_usages.push_back(HIDInputType::Button);
                }    
                
                for (HIDUsage &usage : current_usages)
                {
                    if (item->GetType() == HIDItemType::HID_USAGE_MINIMUM)
                        usage.usage_min = item->GetValueUint32();
                    else if (item->GetType() == HIDItemType::HID_USAGE_MAXIMUM)
                        usage.usage_max = item->GetValueUint32();
                } 
                break;
            }
            
            case HIDItemType::HID_REPORT_ID:
            {
                if (current_report->report_id == 0)
                    current_report->report_id = item->GetValueUint32();
                break;
            }

            case HIDItemType::HID_LOGICAL_MINIMUM:
            case HIDItemType::HID_LOGICAL_MAXIMUM:
            case HIDItemType::HID_PHYSICAL_MINIMUM:
            case HIDItemType::HID_PHYSICAL_MAXIMUM:
            case HIDItemType::HID_UNIT_EXPONENT:
            case HIDItemType::HID_UNIT:
            case HIDItemType::HID_REPORT_SIZE:
            case HIDItemType::HID_REPORT_COUNT:
            {
                if (item->GetType() == HIDItemType::HID_LOGICAL_MINIMUM)
                    current_property.logical_min = item->GetValueUint32();
                else if (item->GetType() == HIDItemType::HID_LOGICAL_MAXIMUM)
                    current_property.logical_max = item->GetValueUint32();
                else if (item->GetType() == HIDItemType::HID_PHYSICAL_MINIMUM)
                    current_property.physical_min = item->GetValueUint32();
                else if (item->GetType() == HIDItemType::HID_PHYSICAL_MAXIMUM)
                    current_property.physical_max = item->GetValueUint32();
                else if (item->GetType() == HIDItemType::HID_UNIT_EXPONENT)
                    current_property.unit_exponent = item->GetValueUint32();
                else if (item->GetType() == HIDItemType::HID_UNIT)
                    current_property.unit = item->GetValueUint32();
                else if (item->GetType() == HIDItemType::HID_REPORT_SIZE)
                    current_property.size = item->GetValueUint32();
                else if (item->GetType() == HIDItemType::HID_REPORT_COUNT)
                    current_property.count = item->GetValueUint32();
               
                break;
            }

            case HIDItemType::HID_INPUT:
            {
                if (current_usages.size() == 0)
                    current_usages.push_back(HIDUsage(HIDInputType::Padding));

                for (HIDUsage &usage : current_usages)
                {
                    assert((current_property.count == current_usages.size()) || (current_usages.size() == 1));
                    
                    uint32_t count = current_property.count / (uint32_t)current_usages.size();
                    for (uint32_t i = 0; i < count; i++)
                    {
                        HIDInput input = current_property;
                        input.type = usage.type;
                        input.id = usage.usage_min + i;
                        current_report->inputs.push_back(input);
                    }
                }
                current_usages.clear();
                break;
            }

            /*case HIDItemType::HID_OUTPUT:
            {
                for (HIDUsage &usage : current_usages)
                {
                    HIDOutput output = current_property;
                    output.type = usage.type;
                    output.count /= (uint32_t)current_usages.size(); //divide by the number of usages
                    current_report->outputs.push_back(output);
                }

                current_usages.clear();
                break;
            }*/

                //For now collections are ignored
            case HIDItemType::HID_COLLECTION:
            {
                current_usages.clear();
                
                collection_stack.push(std::make_shared<HIDCollection>((HIDCollectionType)(item->GetValueUint32())));
                break;
            }

            case HIDItemType::HID_END_COLLECTION:
            {
                collection_stack.pop();

                if (collection_stack.size() == 0)
                {
                    m_reports.push_back(current_report);
                    current_report = nullptr;
                }

                break;
            }
        }
    }

    assert(current_report == nullptr);
    assert(collection_stack.size() == 0);    
}

/* -------------------------------------------------------------------- */

std::vector<std::shared_ptr<HIDItem>> HIDReportDescriptor::parseItems(const uint8_t *hid_report_data, uint16_t hid_report_data_len)
{
    std::vector<std::shared_ptr<HIDItem>> items;

    for (uint16_t offset = 0; offset < hid_report_data_len; /* ... */)
    {
        uint8_t item_type = hid_report_data[offset];
        uint8_t datalen = item_type & HID_LENGTH_MASK;
        if (datalen == 3) datalen = 4;

        std::shared_ptr<HIDItem> item = std::make_shared<HIDItem>((HIDItemType)(item_type & HID_FUNC_TYPE_MASK), &hid_report_data[offset + 1], datalen);
        offset += 1 + datalen;
        
        items.push_back(item);
    }

    return items;  
}