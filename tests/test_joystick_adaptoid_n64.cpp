#include <gtest/gtest.h>
#include "HIDReportDescriptor.h"
#include "HIDJoystick.h"

const uint8_t report_data[] = {
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x04,        // Usage (Joystick)
    0xA1, 0x01,        // Collection (Application)
    0x09, 0x01,        //   Usage (Pointer)
    0xA1, 0x00,        //   Collection (Physical)
    0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
    0x09, 0x30,        //     Usage (X)
    0x09, 0x31,        //     Usage (Y)
    0x16, 0x50, 0xFB,  //     Logical Minimum (-1200)
    0x26, 0xB0, 0x04,  //     Logical Maximum (1200)
    0x36, 0x00, 0x00,  //     Physical Minimum (0)
    0x46, 0x60, 0x09,  //     Physical Maximum (2400)
    0x75, 0x0C,        //     Report Size (12)
    0x95, 0x02,        //     Report Count (2)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              //   End Collection
    0x05, 0x09,        //   Usage Page (Button)
    0x19, 0x01,        //   Usage Minimum (0x01)
    0x29, 0x0E,        //   Usage Maximum (0x0E)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x01,        //   Logical Maximum (1)
    0x35, 0x00,        //   Physical Minimum (0)
    0x45, 0x01,        //   Physical Maximum (1)
    0x75, 0x01,        //   Report Size (1)
    0x95, 0x0E,        //   Report Count (14)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x02,        //   Report Count (2)
    0x75, 0x01,        //   Report Size (1)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection
};

TEST(ADAPTOID, test_report_has_1_joystick)
{
    HIDReportDescriptor hid_report_descriptor(report_data, (uint16_t)sizeof(report_data));
    GTEST_ASSERT_EQ(hid_report_descriptor.get_reports().size(), 1);
    GTEST_ASSERT_EQ(hid_report_descriptor.get_reports()[0].report_type, HIDIOReportType::Joystick);
}

TEST(ADAPTOID, test_input_parsing_centered)
{
    // byte0-2: X=0x000, Y=0x000 (bit-packed 12+12) / byte3-4: no buttons pressed
    uint8_t data[5] = {0x00, 0x00, 0x00, 0x00, 0x00};

    HIDJoystick joystick(std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data)));
    HIDJoystickData joystick_data;

    GTEST_ASSERT_EQ(joystick.parse_data(data, sizeof(data), &joystick_data), true);
    GTEST_ASSERT_EQ(joystick_data.x, -1);
    GTEST_ASSERT_EQ(joystick_data.y, -1);
}

TEST(ADAPTOID, test_input_parsing_positive_max)
{
    // X = 0x4B0 (bits 0-11) -> byte0 = 0xB0, low nibble of byte1 = 0x4
    uint8_t data[5] = {0xB0, 0x04, 0x00, 0x00, 0x00};

    HIDJoystick joystick(std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data)));
    HIDJoystickData joystick_data;

    GTEST_ASSERT_EQ(joystick.parse_data(data, sizeof(data), &joystick_data), true);
    GTEST_ASSERT_EQ(joystick_data.x, 32767);
}

TEST(ADAPTOID, test_input_parsing_negative_min)
{
    // X = 0xB50 (bits 0-11) -> byte0 = 0x50, low nibble of byte1 = 0xB
    uint8_t data[5] = {0x50, 0x0B, 0x00, 0x00, 0x00};

    HIDJoystick joystick(std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data)));
    HIDJoystickData joystick_data;

    GTEST_ASSERT_EQ(joystick.parse_data(data, sizeof(data), &joystick_data), true);
    GTEST_ASSERT_EQ(joystick_data.x, -32768);
}

TEST(ADAPTOID, test_input_parsing_y_mid_negative)
{
    // Y bits 12-23: low nibble = 0x8 goes into high nibble of byte1, then byte2 = 0xDA
    uint8_t data[5] = {0x00, 0x80, 0xDA, 0x00, 0x00};

    HIDJoystick joystick(std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data)));
    HIDJoystickData joystick_data;

    GTEST_ASSERT_EQ(joystick.parse_data(data, sizeof(data), &joystick_data), true);
    GTEST_ASSERT_EQ(joystick_data.x, -1); // see test_input_parsing_centered re: truncation at raw 0

    // -600 out of [-1200,1200] mapped to [-32768,32767] ~= -16384
    EXPECT_NEAR(joystick_data.y, -16384, 5);
}

TEST(ADAPTOID, test_input_parsing_buttons)
{
    // Press the 1st button (bit0 of byte3, HID id 1) and the 14th button (bit5 of byte4, HID id 14)
    uint8_t data[5] = {0x00, 0x00, 0x00, 0x01, 0x20};

    HIDJoystick joystick(std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data)));
    HIDJoystickData joystick_data;

    GTEST_ASSERT_EQ(joystick.parse_data(data, sizeof(data), &joystick_data), true);
    GTEST_ASSERT_EQ(joystick_data.buttons[1], true);
    GTEST_ASSERT_EQ(joystick_data.buttons[14], true);
    GTEST_ASSERT_EQ(joystick_data.buttons[2], false);
}