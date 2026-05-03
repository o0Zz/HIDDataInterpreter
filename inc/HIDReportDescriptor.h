#pragma once

#include <stdint.h>
#include <vector>

enum class HIDIOType 
{
    Unknown = 0x00,
    ReportId,
    VendorDefined,
    Padding,
    Button,
    Consumer,
    X,
    Y,
    Z,
    Rx,
    Ry,
    Rz,
    Slider,
    Dial,
    Brake,
    Accelerator,
    HatSwitch,
    Wheel,
    Key
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

    int32_t logical_min;
    int32_t logical_max;
    int32_t physical_min;
    int32_t physical_max;
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

#define HID_DEVICE_POINTER   0x0001
#define HID_DEVICE_MOUSE     0x0002
#define HID_DEVICE_JOYSTICK  0x0004
#define HID_DEVICE_GAMEPAD   0x0008
#define HID_DEVICE_KEYBOARD  0x0010
#define HID_DEVICE_KEYPAD    0x0020
#define HID_DEVICE_MULTIAXIS 0x0040
#define HID_DEVICE_TABLET    0x0080


//https://usb.org/sites/default/files/hut1_2.pdf p118
enum class HIDIOConsumerType
{
    Menu = 0x40,
    RedMenuButton = 0x69,
    GreenMenuButton = 0x6A,
    BlueMenuButton = 0x6B,
    YellowMenuButton = 0x6C,
    VolumeIncrement = 0xE9,
    VolumeDecrement = 0xEA,
    PlayPause = 0xCD
};

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
    HIDReportDescriptor();
    HIDReportDescriptor(const uint8_t *hid_report_data, uint16_t hid_report_data_size);
    ~HIDReportDescriptor();

    std::vector<HIDIOReport> get_reports() const { return m_reports; }
    uint16_t get_device_types() const;
    
private:
    void parse(const uint8_t *hid_report_data, uint16_t hid_report_data_len);

    std::vector<HIDIOReport> m_reports;
};
