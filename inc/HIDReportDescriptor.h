#pragma once

#include <stdint.h>
#include <memory>
#include <vector>
#include <string>

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
class HIDUsage;

class HIDInputOutput
{
public:
    HIDInputOutput(HIDIOType type = HIDIOType::Unknown, uint32_t size=0, uint32_t id=0);
    ~HIDInputOutput();
    HIDInputOutput(const HIDUsage &usage, uint32_t idx);

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

//https://usb.org/sites/default/files/hut1_2.pdf p31
typedef enum class HIDIOReportType
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
} HIDIOReportType;

class HIDIOBlock
{
public:
    HIDIOBlock() {}
    ~HIDIOBlock() {}

    std::vector<HIDInputOutput> data;
};

class HIDIOReport
{
public:
    HIDIOReport(HIDIOReportType report_type = HIDIOReportType::Unknown) :
        report_type(report_type)
    {}

    HIDIOReportType report_type;
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

    std::vector<std::shared_ptr<HIDIOReport>> GetReports() const { return m_reports; }
    
private:
    void parse(const uint8_t *hid_report_data, uint16_t hid_report_data_len);

    std::vector<std::shared_ptr<HIDIOReport>> m_reports;
};
