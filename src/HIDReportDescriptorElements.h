#pragma once
#include <memory>
#include <vector>

/* -------------------------------------------------------------------------- */

typedef enum class HIDElementType
{
    HID_UNKNOWN = 0x00,
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
} HIDElementType;

/* -------------------------------------------------------------------------- */

class HIDElement
{
public:
    HIDElement();
    HIDElement(HIDElementType type, const uint8_t *data, uint8_t data_size);
    ~HIDElement();

    HIDElementType GetType() const;
    uint32_t GetValueUint32() const;

private:
    HIDElementType type;
    uint8_t data[4];
    uint8_t data_size;
};

/* -------------------------------------------------------------------------- */

class HIDReportDescriptorElements
{
public:
    static std::vector<HIDElement> parse(const uint8_t *hid_report_data, uint16_t hid_report_data_len);
};
