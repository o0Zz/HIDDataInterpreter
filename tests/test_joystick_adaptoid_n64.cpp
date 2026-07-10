#include <gtest/gtest.h>
#include "HIDReportDescriptor.h"
#include "HIDJoystick.h"

// Wish Technologies Adaptoid (N64-controller-to-USB-HID adapter), VID/PID 06F7:0001.
// Descriptor captured from real hardware, see:
// https://github.com/Maschell/controller_patcher/issues/23
//
// Stick: 2x Report Count, 12 bits each (X then Y), Logical Minimum/Maximum -1200/1200
// (two's-complement, non-byte-aligned - Y starts mid-byte).
// Buttons: 14 buttons (1 bit each) + 2 constant padding bits.
const uint8_t adaptoid_report_data[] = {
    0x05, 0x01, 0x09, 0x04, 0xa1, 0x01, 0x09, 0x01, 0xa1, 0x00, 0x05, 0x01, 0x09, 0x30, 0x09, 0x31,
    0x16, 0x50, 0xfb, 0x26, 0xb0, 0x04, 0x36, 0x00, 0x00, 0x46, 0x60, 0x09, 0x75, 0x0c, 0x95, 0x02,
    0x81, 0x02, 0xc0, 0x05, 0x09, 0x19, 0x01, 0x29, 0x0e, 0x15, 0x00, 0x25, 0x01, 0x35, 0x00, 0x45,
    0x01, 0x75, 0x01, 0x95, 0x0e, 0x81, 0x02, 0x95, 0x02, 0x75, 0x01, 0x81, 0x01, 0xc0,
};

TEST(ADAPTOID, test_report_has_1_joystick)
{
    HIDReportDescriptor hid_report_descriptor(adaptoid_report_data, (uint16_t)sizeof(adaptoid_report_data));
    GTEST_ASSERT_EQ(hid_report_descriptor.GetReports().size(), 1);
    GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].report_type, HIDIOReportType::Joystick);
}

// Both stick axes centered (raw 12-bit value 0x000 for X and Y). mapValue()'s integer division
// truncates the exact center of an asymmetric [-1200,1200] -> [-32768,32767] mapping to -1, not 0
// (78642000 / 2400 == 32767 in integer math, then -32768); that's pre-existing mapValue() rounding
// behavior, not something introduced by sign-extension, so we assert the actual value.
TEST(ADAPTOID, test_input_parsing_centered)
{
    // byte0-2: X=0x000, Y=0x000 (bit-packed 12+12) / byte3-4: no buttons pressed
    uint8_t data[5] = {0x00, 0x00, 0x00, 0x00, 0x00};

    HIDJoystick joystick(std::make_shared<HIDReportDescriptor>(adaptoid_report_data, (uint16_t)sizeof(adaptoid_report_data)));
    HIDJoystickData joystick_data;

    GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), true);
    GTEST_ASSERT_EQ(joystick_data.X, -1);
    GTEST_ASSERT_EQ(joystick_data.Y, -1);
}

// X at its declared Logical Maximum (raw 12-bit value 1200 = 0x4B0) should map to full positive range.
TEST(ADAPTOID, test_input_parsing_positive_max)
{
    // X = 0x4B0 (bits 0-11) -> byte0 = 0xB0, low nibble of byte1 = 0x4
    uint8_t data[5] = {0xB0, 0x04, 0x00, 0x00, 0x00};

    HIDJoystick joystick(std::make_shared<HIDReportDescriptor>(adaptoid_report_data, (uint16_t)sizeof(adaptoid_report_data)));
    HIDJoystickData joystick_data;

    GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), true);
    GTEST_ASSERT_EQ(joystick_data.X, 32767);
}

// X at its declared Logical Minimum (raw 12-bit two's-complement value -1200 = 0xB50) should map to
// full negative range. This is the case that exposed the missing sign-extension: without it, the raw
// unsigned bit pattern (2896) gets passed straight into mapValue() against a signed [-1200,1200]
// range and overflows int16_t on assignment.
TEST(ADAPTOID, test_input_parsing_negative_min)
{
    // X = 0xB50 (bits 0-11) -> byte0 = 0x50, low nibble of byte1 = 0xB
    uint8_t data[5] = {0x50, 0x0B, 0x00, 0x00, 0x00};

    HIDJoystick joystick(std::make_shared<HIDReportDescriptor>(adaptoid_report_data, (uint16_t)sizeof(adaptoid_report_data)));
    HIDJoystickData joystick_data;

    GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), true);
    GTEST_ASSERT_EQ(joystick_data.X, -32768);
}

// Y is packed starting mid-byte (bit offset 12): high nibble of byte1 + all of byte2.
// Encode Y = -600 (raw 12-bit two's complement = 4096-600 = 3496 = 0xDA8).
TEST(ADAPTOID, test_input_parsing_y_mid_negative)
{
    // Y bits 12-23: low nibble = 0x8 goes into high nibble of byte1, then byte2 = 0xDA
    uint8_t data[5] = {0x00, 0x80, 0xDA, 0x00, 0x00};

    HIDJoystick joystick(std::make_shared<HIDReportDescriptor>(adaptoid_report_data, (uint16_t)sizeof(adaptoid_report_data)));
    HIDJoystickData joystick_data;

    GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), true);
    GTEST_ASSERT_EQ(joystick_data.X, -1); // see test_input_parsing_centered re: truncation at raw 0

    // -600 out of [-1200,1200] mapped to [-32768,32767] ~= -16384
    EXPECT_NEAR(joystick_data.Y, -16384, 5);
}

// The descriptor declares "Usage Minimum (1)" for buttons, so the parser's `id = usage_min + idx`
// (see HIDReportDescriptor.cpp) numbers the 14 physical buttons 1..14, not 0..13 - buttons[0] is
// never populated for this device. Matches the convention already used by e.g. XBOX360's
// test_input_parsing_button1, which also asserts on buttons[1] for its first Usage Minimum(1) bit.
TEST(ADAPTOID, test_input_parsing_buttons)
{
    // Press the 1st button (bit0 of byte3, HID id 1) and the 14th button (bit5 of byte4, HID id 14)
    uint8_t data[5] = {0x00, 0x00, 0x00, 0x01, 0x20};

    HIDJoystick joystick(std::make_shared<HIDReportDescriptor>(adaptoid_report_data, (uint16_t)sizeof(adaptoid_report_data)));
    HIDJoystickData joystick_data;

    GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), true);
    GTEST_ASSERT_EQ(joystick_data.buttons[1], true);
    GTEST_ASSERT_EQ(joystick_data.buttons[14], true);
    GTEST_ASSERT_EQ(joystick_data.buttons[2], false);
}
