#include "HIDReportDescriptor.h"
#include <iostream>
#include <vector>
#include <memory>
#include <stack>

//Based on: https://github.com/listff/hidparser/blob/main/hid.cpp

//https://github.com/pasztorpisti/hid-report-parser/blob/master/src/hid_report_parser.cpp

typedef enum class HIDEntryType
{
    HID_INPUT = 0x80,
    HID_OUTPUT = 0x90,
    HID_FEATURE = 0xb0,
    HID_COLLECTION = 0xa0,
    HID_END_COLLECTION = 0xc0,
    HID_USAGE_PAGE = 0x04,
    HID_LOGICAL_MINIMUM = 0x14,
    HID_LOGICAL_MAXIMUM = 0x24,
    HID_PHYSICAL_MINIMUM = 0x34,
    HID_PHYSICAL_MAXIMUM = 0x44,
    HID_UNIT_EXPONENT = 0x54,
    HID_UNIT = 0x64,
    HID_REPORT_SIZE = 0x74,
    HID_REPORT_ID = 0x84,
    HID_REPORT_COUNT = 0x94,
    HID_PUSH = 0xa4,
    HID_POP = 0xb4,
    HID_USAGE = 0x08,
    HID_USAGE_MINIMUM = 0x18,
    HID_USAGE_MAXIMUM = 0x28,
    HID_DESIGNATOR_INDEX = 0x38,
    HID_DESIGNATOR_MINIMUM = 0x48,
    HID_DESIGNATOR_MAXIMUM = 0x58,
    HID_STRING_INDEX = 0x78,
    HID_STRING_MINIMUM = 0x88,
    HID_STRING_MAXIMUM = 0x98,
    HID_DELIMITER = 0xa8
} HIDEntryType;



//---------------COLLECTION-----------------
#define Application         0x01

//---------------USAGE_PAGE-----------------
#define USAGE_PAGE_GenericDesktop       0x01
#define USAGE_PAGE_Keyboard             0x07
#define USAGE_PAGE_LEDs                 0x08
#define USAGE_PAGE_Button               0x09

//---------------USAGE-----------------
//https://usb.org/sites/default/files/hut1_2.pdf p31
#define USAGE_Pointer       0x01
#define USAGE_Mouse         0x02
#define USAGE_Joystick      0x04
#define USAGE_GamePad       0x05
#define USAGE_Keyboard      0x06
#define USAGE_Keypad        0x07
#define USAGE_MultiAxis     0x08
#define USAGE_Tablet        0x09

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

//---------------COLLECTION-----------------


//---------------INPUT-----------------
#define INPUT_Const         0x01
#define INPUT_Var           0x02
#define INPUT_Rel           0x04
#define INPUT_Wrap          0x08
#define INPUT_NLin          0x10
#define INPUT_NPrf          0x20
#define INPUT_Null          0x40
#define INPUT_Vol           0x80

class HIDEntry
{
public:
    HIDEntryType type;
    uint8_t value[4] = {0};
    uint8_t data_size;

    HIDEntry() {}
    ~HIDEntry() {}

    HIDEntry(HIDEntryType type, const uint8_t *value, uint8_t data_size)
    {
        this->type = type;
        memcpy(this->value, value, data_size);
        this->data_size = data_size;
    }

    uint32_t GetValueUint32()
    {
        return value[0] | ((uint32_t)value[1] << 8) | ((uint32_t)value[2] << 16) | ((uint32_t)value[3] << 24);
    }
};


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
    std::vector<std::shared_ptr<HIDEntry>> hid_report_descriptior = parseKeyValues(hid_report_data, hid_report_data_len);

    std::vector<HIDInput> current_inputs;
    std::stack<std::shared_ptr<HIDEntry>> usage_page_stack;
    std::stack<std::shared_ptr<HIDCollection>> collection_stack;

    //iterate on hid_report_descriptior
    for (std::shared_ptr<HIDEntry> entry : hid_report_descriptior)
    {
        switch (entry->type)
        {
            case HIDEntryType::HID_USAGE_PAGE:
            {
                //add current_inputs to m_inputs
                m_inputs.insert(m_inputs.end(), current_inputs.begin(), current_inputs.end());
                current_inputs.clear();
                
                if (usage_page_stack.size() > 0)
                    usage_page_stack.pop();

                usage_page_stack.push(entry);
                
                if (entry->GetValueUint32() == USAGE_PAGE_Button)
                {
                    HIDInput input;
                    input.type = HIDInputType::Button;
                    current_inputs.push_back(input);
                }
                
                break;


            }
            case HIDEntryType::HID_USAGE:
            {
                if (entry->GetValueUint32() == USAGE_X)
                {
                    HIDInput input;
                    input.type = HIDInputType::X;
                    current_inputs.push_back(input);
                }
                else if (entry->GetValueUint32() == USAGE_Y)
                {
                    HIDInput input;
                    input.type = HIDInputType::Y;
                    current_inputs.push_back(input);
                }
                else if (entry->GetValueUint32() == USAGE_Hat_switch)
                {
                    HIDInput input;
                    input.type = HIDInputType::HatSwitch;
                    current_inputs.push_back(input);
                }
                
                break;
            }
            
            case HIDEntryType::HID_REPORT_ID:
            {
                HIDInput input;
                input.type = HIDInputType::ReportID;
                input.size =  8;
                m_inputs.push_back(input);
                break;
            }

            case HIDEntryType::HID_LOGICAL_MINIMUM:
            case HIDEntryType::HID_LOGICAL_MAXIMUM:
            case HIDEntryType::HID_PHYSICAL_MINIMUM:
            case HIDEntryType::HID_PHYSICAL_MAXIMUM:
            case HIDEntryType::HID_UNIT_EXPONENT:
            case HIDEntryType::HID_UNIT:
            case HIDEntryType::HID_REPORT_SIZE:
            case HIDEntryType::HID_REPORT_COUNT:
            {
                for (HIDInput &input : current_inputs)
                {
                    if (entry->type == HIDEntryType::HID_LOGICAL_MINIMUM)
                        input.logical_min = entry->GetValueUint32();
                    else if (entry->type == HIDEntryType::HID_LOGICAL_MAXIMUM)
                        input.logical_max = entry->GetValueUint32();
                    else if (entry->type == HIDEntryType::HID_PHYSICAL_MINIMUM)
                        input.physical_min = entry->GetValueUint32();
                    else if (entry->type == HIDEntryType::HID_PHYSICAL_MAXIMUM)
                        input.physical_max = entry->GetValueUint32();
                    else if (entry->type == HIDEntryType::HID_UNIT_EXPONENT)
                        input.unit_exponent = entry->GetValueUint32();
                    else if (entry->type == HIDEntryType::HID_UNIT)
                        input.unit = entry->GetValueUint32();
                    else if (entry->type == HIDEntryType::HID_REPORT_SIZE)
                        input.size = entry->GetValueUint32();
                    else if (entry->type == HIDEntryType::HID_REPORT_COUNT)
                        input.count = entry->GetValueUint32();
                }
                break;
            }

                //For now collections are ignored
            case HIDEntryType::HID_COLLECTION:
            {
                std::shared_ptr<HIDCollection> collection = std::make_shared<HIDCollection>((HIDCollectionType)(entry->GetValueUint32()));
                collection_stack.push(collection);
                break;
            }
            case HIDEntryType::HID_END_COLLECTION:
            {
                m_inputs.insert(m_inputs.end(), current_inputs.begin(), current_inputs.end());
                current_inputs.clear();

                collection_stack.pop();
                break;
            }
        }
    }
}

#define HID_KEY_MASK 0xFC
#define HID_LENGTH_MASK 0x03

std::vector<std::shared_ptr<HIDEntry>> HIDReportDescriptor::parseKeyValues(const uint8_t *hid_report_data, uint16_t hid_report_data_len)
{
    std::vector<std::shared_ptr<HIDEntry>> hid_report_descriptior;
    uint16_t offset = 0;

    for (;;)
    {
        if (offset >= hid_report_data_len)
            break;

        uint8_t key = hid_report_data[offset];
        uint8_t datalen = key & HID_LENGTH_MASK;
        if (datalen == 3) datalen = 4;

        std::shared_ptr<HIDEntry> item = std::make_shared<HIDEntry>((HIDEntryType)(key & HID_KEY_MASK), &hid_report_data[offset + 1], datalen);
        offset += 1 + datalen;
        
        hid_report_descriptior.push_back(item);
    }

    return hid_report_descriptior;  
}



/*
void printhid(uchar* hid, int hidlen)
{
    unsigned char level = 0;
    uint32_t usage = 0, usagepage = 0;

    for (int i = 0; i < hidlen;)
    {
        unsigned char cmd = hid[i];
        uchar datalen = 0;
        uint32_t cmdtype = 0;

        switch (cmd & 3)
        {
        case 3:
        {
            datalen = 4;
        }break;

        default:
        {
            datalen = cmd & 3;
        }break;
        }

        if (cmd == 0xc0)
        {
            for (int p = 0; p < (level - 1); p++)
            {
                printf("\t");
            }
        }
        else
        {
            for (int p = 0; p < level; p++)
            {
                printf("\t");
            }
        }


        for (int p = 0; p < (datalen + 1); p++)
        {
            printf("0x%02x ", hid[i + p]);
        }

        i += 1;

        switch ((cmd >> 2) & 0x3)
        {
        case 0:
        {
            switch (cmd >> 4)
            {
            case 8:
            {
                printf("Input");
                cmdtype = 3;
            }break;
            case 9:
            {
                printf("Output");
                cmdtype = 3;
            }break;
            case 0xb:
            {
                printf("Feature");
            }break;
            case 0xa:
            {
                level += 1;
                printf("Collection");
                cmdtype = 4;
            }break;
            case 0xc:
            {
                level -= 1;
                printf("End Collection");
            }break;
            default:
            {
                printf("unk!!!");
            }break;
            }
        }break;

        case 1:
        {
            switch (cmd >> 4)
            {
            case 0:
            {
                printf("Usage Page");
                cmdtype = 2;
            }break;
            case 1:
            {
                printf("Logical Minimum");
            }break;
            case 2:
            {
                printf("Logical Maximum");
            }break;
            case 3:
            {
                printf("Physical Minimum");
            }break;
            case 4:
            {
                printf("Physical Maximum");
            }break;
            case 5:
            {
                printf("Unit Exponent");
            }break;
            case 6:
            {
                printf("Unit");
            }break;
            case 7:
            {
                printf("Report Size");
            }break;
            case 8:
            {
                printf("Report ID");
            }break;
            case 9:
            {
                printf("Report Count");
            }break;
            case 0xa:
            {
                printf("Push");
            }break;
            case 0xb:
            {
                printf("Pop");
            }break;
            default:
            {
                printf("unk!!!");
            }break;
            }
        }break;

        case 2:
        {
            switch (cmd >> 4)
            {
            case 0:
            {
                printf("Usage");
                cmdtype = 1;
            }break;
            case 1:
            {
                printf("Usage Minimum");
            }break;
            case 2:
            {
                printf("Usage Maximum");
            }break;
            case 3:
            {
                printf("Designator Index");
            }break;
            case 4:
            {
                printf("Designator Minimum");
            }break;
            case 5:
            {
                printf("Designator Maximum");
            }break;
            case 7:
            {
                printf("String Index");
            }break;
            case 8:
            {
                printf("String Minimum");
            }break;
            case 9:
            {
                printf("String Maximum");
            }break;
            case 0xa:
            {
                printf("Delimiter");
            }break;
            default:
            {
                printf("unk!!!");
            }break;
            }
        }break;

        }

        uint32_t data = 0;

        memcpy(&data, &hid[i], datalen);

        switch (cmdtype)
        {
        case 1://usage
        {
            usage = data;
            printf(" (%s)", getusagename(usagepage, usage));
        }break;

        case 2://usage page
        {
            usagepage = data;
            printf(" (%s)", getpagename(usagepage));
        }break;

        case 3:
        {
            printf(" (");

            if (data & INPUT_Const)
            {
                printf("Cnst");
            }
            else
            {
                printf("Data");
            }

            if (data & INPUT_Var)
            {
                printf(",Var");
            }
            else
            {
                printf(",Ary");
            }

            if (data & INPUT_Rel)
            {
                printf(",Rel");
            }
            else
            {
                printf(",Abs");
            }

            if (data & INPUT_Wrap)
            {
                printf(",Wrap");
            }

            if (data & INPUT_NLin)
            {
                printf(",NLin");
            }

            if (data & INPUT_NPrf)
            {
                printf(",NPrf");
            }
            if (data & INPUT_Null)
            {
                printf(",Null");
            }

            if (data & INPUT_Vol)
            {
                printf(",Vol");
            }

            printf(")");

        }break;

        case 4:
        {
            switch (data)
            {
            case 0:
            {
                printf(" (Physical)");
            }break;

            case 1:
            {
                printf(" (Application)");
            }break;

            case 2:
            {
                printf(" (Logical)");
            }break;

            }
        }break;

        default:
        {
            switch (datalen)
            {
            case 1:
            {
                printf(" (%d)", *(char*)&hid[i]);
            }break;

            case 2:
            {
                printf(" (%d)", *(short*)&hid[i]);
            }break;
            case 4:
            {
                printf(" (%d)", *(int*)&hid[i]);
            }break;
            }
        }
        break;
        }


        printf("\r\n");

        i += datalen;
    }
}*/