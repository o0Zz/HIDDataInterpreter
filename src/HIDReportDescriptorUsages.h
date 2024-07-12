#pragma once
#include "HIDReportDescriptorElements.h"

enum class HIDUsageIOType 
{
    None = 0x00,
    Input,
    Output,
    Feature
};

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
    Pointer = 0x01,
    Mouse = 0x02,
    Joystick = 0x04,
    GamePad = 0x05,
    Keyboard = 0x06,
    Keypad = 0x07,
    MultiAxisController = 0x08,

    ReportTypeEnd = 0x1F,
    
    X           = 0x30,
    Y           = 0x31,
    Z           = 0x32,
    Rx          = 0x33,
    Ry          = 0x34,
    Rz          = 0x35,
    Wheel       = 0x38,
    HatSwitch   = 0x39,
};

class HIDProperty
{
public:
    HIDProperty(uint32_t size=0, uint32_t count=0);
    ~HIDProperty();

    bool is_valid();

    uint32_t logical_min;
    uint32_t logical_max;
    uint32_t physical_min;
    uint32_t physical_max;
    uint32_t unit;
    uint32_t unit_exponent;
    uint32_t size; //Size of the data in bits
    uint32_t count; //Number of data items
};

class HIDUsage
{
public:
    /// @brief 
    /// @param type 
    /// @param sub_type will depend on the type, for example, if type is GenericDesktop, sub_type will be HIDUsageGenericDesktopSubType etc.
    /// @param property 
    HIDUsage(HIDUsageType type, uint32_t sub_type = 0, HIDUsageIOType io_type = HIDUsageIOType::None, HIDProperty property = HIDProperty());
    ~HIDUsage();

    HIDUsageType type; //Input type (Button, X, Y, Hat switch, Padding, etc.)
    uint32_t sub_type; //Sub type (Button number, etc.)
    uint32_t usage_min;
    uint32_t usage_max;
    HIDUsageIOType io_type;
    HIDProperty property;
};

class HIDReport
{
public:
    std::vector<HIDUsage> usages;
};

class HIDReportDescriptorUsages
{
public:
    static std::vector<HIDReport> parse(const HIDReportDescriptorElements &elements);
};