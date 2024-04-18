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


HIDReportDescriptor::HIDReportDescriptor(const uint8_t *hid_report_data, uint16_t hid_report_data_len)
{
    parse(hid_report_data, hid_report_data_len);
}


HIDReportDescriptor::~HIDReportDescriptor()
{

}

void HIDReportDescriptor::parse(const uint8_t *hid_report_data, uint16_t hid_report_data_len)
{
    std::vector<std::shared_ptr<HIDItem>> hid_report_items = parseItems(hid_report_data, hid_report_data_len);

    std::shared_ptr<HIDReport> current_report = nullptr;
    
    std::vector<HIDInput> current_inputs;
    std::shared_ptr<HIDItem> current_usage_page;
    std::stack<std::shared_ptr<HIDCollection>> collection_stack;

    //iterate on hid_report_descriptior
    for (std::shared_ptr<HIDItem> item : hid_report_items)
    {
        switch (item->GetType())
        {
            case HIDItemType::HID_USAGE_PAGE:
            {
                assert(current_inputs.size() == 0);

                current_usage_page = item;
                
                if (item->GetValueUint32() == USAGE_PAGE_Button)
                {
                    current_inputs.push_back(HIDInput(HIDInputType::Button));
                }
                
                break;
            }

            case HIDItemType::HID_USAGE:
            {
                if (current_usage_page->GetValueUint32() == USAGE_PAGE_GenericDesktop)
                {
                    if (item->GetValueUint32() < (uint32_t)HIDDeviceType::MAX)
                    {
                        if (current_report == nullptr)
                            current_report = std::make_shared<HIDReport>((HIDDeviceType)item->GetValueUint32());
                    }
                    else
                    {
                        current_inputs.push_back(HIDInput((HIDInputType)item->GetValueUint32()));
                    }
                }

                break;
            }
            
            case HIDItemType::HID_REPORT_ID:
            {
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
                if (current_inputs.size() == 0)
                    current_inputs.push_back(HIDInput(HIDInputType::Padding));

                for (HIDInput &input : current_inputs)
                {
                    if (item->GetType() == HIDItemType::HID_LOGICAL_MINIMUM)
                        input.logical_min = item->GetValueUint32();
                    else if (item->GetType() == HIDItemType::HID_LOGICAL_MAXIMUM)
                        input.logical_max = item->GetValueUint32();
                    else if (item->GetType() == HIDItemType::HID_PHYSICAL_MINIMUM)
                        input.physical_min = item->GetValueUint32();
                    else if (item->GetType() == HIDItemType::HID_PHYSICAL_MAXIMUM)
                        input.physical_max = item->GetValueUint32();
                    else if (item->GetType() == HIDItemType::HID_UNIT_EXPONENT)
                        input.unit_exponent = item->GetValueUint32();
                    else if (item->GetType() == HIDItemType::HID_UNIT)
                        input.unit = item->GetValueUint32();
                    else if (item->GetType() == HIDItemType::HID_REPORT_SIZE)
                        input.size = item->GetValueUint32();
                    else if (item->GetType() == HIDItemType::HID_REPORT_COUNT)
                        input.count = item->GetValueUint32() / (uint32_t)current_inputs.size(); //divide by the number of inputs
                }
                break;
            }
            case HIDItemType::HID_INPUT:
            {
                current_report->inputs.insert(current_report->inputs.end(), current_inputs.begin(), current_inputs.end());
                current_inputs.clear();
                break;
            }
                //For now collections are ignored
            case HIDItemType::HID_COLLECTION:
            {
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
}

std::vector<std::shared_ptr<HIDItem>> HIDReportDescriptor::parseItems(const uint8_t *hid_report_data, uint16_t hid_report_data_len)
{
    std::vector<std::shared_ptr<HIDItem>> items;
    uint16_t offset = 0;

    for (;;)
    {
        if (offset >= hid_report_data_len)
            break;

        uint8_t item_type = hid_report_data[offset];
        uint8_t datalen = item_type & HID_LENGTH_MASK;
        if (datalen == 3) datalen = 4;

        std::shared_ptr<HIDItem> item = std::make_shared<HIDItem>((HIDItemType)(item_type & HID_FUNC_TYPE_MASK), &hid_report_data[offset + 1], datalen);
        offset += 1 + datalen;
        
        items.push_back(item);
    }

    return items;  
}