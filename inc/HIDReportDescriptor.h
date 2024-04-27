#pragma once
#include <stdint.h>
#include <memory>
#include <vector>
#include <string>

/* -------------------------------------------------------------------------- */

enum class HIDUsageType 
{
    Unknown = 0x00,
    ReportId,
    Padding,
    Button,
    GenericDesktop,
    VendorDefined
};

enum class HIDUsageGenericDesktopSubType 
{
    X           = 0x30,
    Y           = 0x31,
    Z           = 0x32,
    Rx          = 0x33,
    Ry          = 0x34,
    Rz          = 0x35,
    Wheel       = 0x38,
    HatSwitch   = 0x39,
};

class HIDUsage
{
public:
    HIDUsage(HIDUsageType type, uint32_t sub_type = 0) : 
        type(type), 
        sub_type(sub_type),
        usage_min(0), 
        usage_max(0)
        {
            if (sub_type != 0)
            {
                usage_min = sub_type;
                usage_max = sub_type;
            }
        }
    ~HIDUsage() {}

    HIDUsageType type; //Input type (Button, X, Y, Hat switch, Padding, etc.)
    uint32_t sub_type; //Sub type (Button number, etc.)
    uint32_t usage_min;
    uint32_t usage_max;
};

class HIDProperty
{
public:
    HIDProperty() : 
        logical_min(0), 
        logical_max(0), 
        physical_min(0), 
        physical_max(0), 
        unit(0), 
        unit_exponent(0), 
        size(size), 
        count(count) {}
    ~HIDProperty() {}

    uint32_t logical_min;
    uint32_t logical_max;
    uint32_t physical_min;
    uint32_t physical_max;
    uint32_t unit;
    uint32_t unit_exponent;
    uint32_t size; //Size of the data in bits
    uint32_t count; //Number of data items
};

enum class HIDIOType 
{
    Unknown = 0x00,
    ReportId,
    VendorDefined,
    Padding,
    Button,
    X,
    Y,
    Z,
    Rx,
    Ry,
    Rz,
    HatSwitch,
    Wheel
};

class HIDInputOutput
{
public:
    HIDInputOutput(HIDIOType type = HIDIOType::Unknown, uint32_t size=0, uint32_t id=0) : 
        type(type), 
        sub_type(0),
        size(size),
        id(id),
        logical_min(0), 
        logical_max(0), 
        physical_min(0), 
        physical_max(0), 
        unit(0),
        unit_exponent(0) {}
    ~HIDInputOutput() {}

    //ctor from hidpropery
    HIDInputOutput(const HIDUsage &usage, const HIDProperty &p, uint32_t idx) : 
        type(HIDIOType::Unknown),
        sub_type(0),
        size(p.size),
        id(0),
        logical_min(p.logical_min),
        logical_max(p.logical_max),
        physical_min(p.physical_min), 
        physical_max(p.physical_max), 
        unit(p.unit), 
        unit_exponent(p.unit_exponent) 
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

    HIDIOType type; //Type (Button, X, Y, Hat switch, Padding, etc.)
    uint32_t sub_type; //Sub type (Usefull for vendor defined and non handled types)
    uint32_t size; //Size of the data in bits
    uint32_t id; //Index of the input in the report

    uint32_t logical_min;
    uint32_t logical_max;
    uint32_t physical_min;
    uint32_t physical_max;
    uint32_t unit;
    uint32_t unit_exponent;
};

/* -------------------------------------------------------------------------- */

typedef enum class HIDItemType
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
} HIDItemType;

class HIDItem
{
public:
    HIDItem() {}
    ~HIDItem() {}

    HIDItem(HIDItemType type, const uint8_t *data, uint8_t data_size)
    {
        this->type = type;
        memcpy(this->data, data, data_size);
        this->data_size = data_size;
    }

    HIDItemType GetType()
    {
        return type;
    }

    uint32_t GetValueUint32()
    {
        return data[0] | ((uint32_t)data[1] << 8) | ((uint32_t)data[2] << 16) | ((uint32_t)data[3] << 24);
    }

private:
    HIDItemType type;
    uint8_t data[4] = {0};
    uint8_t data_size;
};

/* -------------------------------------------------------------------------- */

//https://usb.org/sites/default/files/hut1_2.pdf p31
typedef enum class HIDReportType
{
    Unknown = 0x00,
    Pointer = 0x01,
    Mouse = 0x02,
    Joystick = 0x04,
    GamePad = 0x05,
    Keyboard = 0x06,
    Keypad = 0x07,
    MultiAxis = 0x08,
    Tablet = 0x09,

    MAX = 0x2F
} HIDReportType;

class HIDIOBlock
{
public:
    HIDIOBlock() {}
    ~HIDIOBlock() {}

    std::vector<HIDInputOutput> data;
};

class HIDReport
{
public:
    HIDReport(HIDReportType report_type = HIDReportType::Unknown) :
        report_type(report_type)
    {}

    HIDReportType           report_type;
    std::vector<HIDIOBlock> inputs;
    std::vector<HIDIOBlock> outputs;
    std::vector<HIDIOBlock> features;
};

/* -------------------------------------------------------------------------- */

class HIDReportDescriptor
{
public:
    HIDReportDescriptor(const uint8_t *hid_report_data, uint16_t hid_report_data_size);
    ~HIDReportDescriptor();

    std::vector<std::shared_ptr<HIDReport>> GetReports() const { return m_reports; }
private:
    void                                   parse(const uint8_t *hid_report_data, uint16_t hid_report_data_len);
    std::vector<std::shared_ptr<HIDItem>>  parseItems(const uint8_t *hid_report_data, uint16_t hid_report_data_len);

    std::vector<std::shared_ptr<HIDReport>> m_reports;
};
