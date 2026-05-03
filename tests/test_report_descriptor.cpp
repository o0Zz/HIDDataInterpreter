#include <gtest/gtest.h>
#include "HIDReportDescriptor.h"

// PS4 GamePad descriptor (from test_joystick_ps4.cpp)
static const uint8_t gamepad_report_data[] = {
	0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
	0x09, 0x05,        // Usage (Game Pad)
	0xA1, 0x01,        // Collection (Application)
	0x85, 0x01,        //   Report ID (1)
	0x09, 0x30,        //   Usage (X)
	0x09, 0x31,        //   Usage (Y)
	0x15, 0x00,        //   Logical Minimum (0)
	0x26, 0xFF, 0x00,  //   Logical Maximum (255)
	0x75, 0x08,        //   Report Size (8)
	0x95, 0x02,        //   Report Count (2)
	0x81, 0x02,        //   Input (Data,Var,Abs)
	0x05, 0x09,        //   Usage Page (Button)
	0x19, 0x01,        //   Usage Minimum (0x01)
	0x29, 0x02,        //   Usage Maximum (0x02)
	0x15, 0x00,        //   Logical Minimum (0)
	0x25, 0x01,        //   Logical Maximum (1)
	0x75, 0x01,        //   Report Size (1)
	0x95, 0x02,        //   Report Count (2)
	0x81, 0x02,        //   Input (Data,Var,Abs)
	0x75, 0x06,        //   Report Size (6)
	0x95, 0x01,        //   Report Count (1)
	0x81, 0x03,        //   Input (Const)
	0xC0,              // End Collection
};

// Mouse descriptor
static const uint8_t mouse_report_data[] = {
	0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
	0x09, 0x02,        // Usage (Mouse)
	0xA1, 0x01,        // Collection (Application)
	0x09, 0x01,        //   Usage (Pointer)
	0xA1, 0x00,        //   Collection (Physical)
	0x05, 0x09,        //     Usage Page (Button)
	0x19, 0x01,        //     Usage Minimum (0x01)
	0x29, 0x03,        //     Usage Maximum (0x03)
	0x15, 0x00,        //     Logical Minimum (0)
	0x25, 0x01,        //     Logical Maximum (1)
	0x75, 0x01,        //     Report Size (1)
	0x95, 0x03,        //     Report Count (3)
	0x81, 0x02,        //     Input (Data,Var,Abs)
	0x75, 0x05,        //     Report Size (5)
	0x95, 0x01,        //     Report Count (1)
	0x81, 0x03,        //     Input (Const)
	0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
	0x09, 0x30,        //     Usage (X)
	0x09, 0x31,        //     Usage (Y)
	0x15, 0x81,        //     Logical Minimum (-127)
	0x25, 0x7F,        //     Logical Maximum (127)
	0x75, 0x08,        //     Report Size (8)
	0x95, 0x02,        //     Report Count (2)
	0x81, 0x06,        //     Input (Data,Var,Rel)
	0xC0,              //   End Collection
	0xC0,              // End Collection
};

// Keyboard descriptor
static const uint8_t keyboard_report_data[] = {
	0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
	0x09, 0x06,        // Usage (Keyboard)
	0xA1, 0x01,        // Collection (Application)
	0x05, 0x07,        //   Usage Page (Keyboard/Keypad)
	0x19, 0xE0,        //   Usage Minimum (0xE0)
	0x29, 0xE7,        //   Usage Maximum (0xE7)
	0x15, 0x00,        //   Logical Minimum (0)
	0x25, 0x01,        //   Logical Maximum (1)
	0x75, 0x01,        //   Report Size (1)
	0x95, 0x08,        //   Report Count (8)
	0x81, 0x02,        //   Input (Data,Var,Abs)
	0x95, 0x01,        //   Report Count (1)
	0x75, 0x08,        //   Report Size (8)
	0x81, 0x03,        //   Input (Const)
	0x95, 0x06,        //   Report Count (6)
	0x75, 0x08,        //   Report Size (8)
	0x15, 0x00,        //   Logical Minimum (0)
	0x25, 0x65,        //   Logical Maximum (101)
	0x05, 0x07,        //   Usage Page (Keyboard/Keypad)
	0x19, 0x00,        //   Usage Minimum (0x00)
	0x29, 0x65,        //   Usage Maximum (0x65)
	0x81, 0x00,        //   Input (Data,Array,Abs)
	0xC0,              // End Collection
};

TEST(REPORT_DESCRIPTOR, test_gamepad_device_type)
{
	HIDReportDescriptor desc(gamepad_report_data, (uint16_t)sizeof(gamepad_report_data));
	uint16_t types = desc.get_device_types();
	EXPECT_TRUE(types & HID_DEVICE_GAMEPAD);
	EXPECT_FALSE(types & HID_DEVICE_MOUSE);
	EXPECT_FALSE(types & HID_DEVICE_KEYBOARD);
}

TEST(REPORT_DESCRIPTOR, test_mouse_device_type)
{
	HIDReportDescriptor desc(mouse_report_data, (uint16_t)sizeof(mouse_report_data));
	uint16_t types = desc.get_device_types();
	EXPECT_TRUE(types & HID_DEVICE_MOUSE);
	EXPECT_FALSE(types & HID_DEVICE_JOYSTICK);
	EXPECT_FALSE(types & HID_DEVICE_KEYBOARD);
}

TEST(REPORT_DESCRIPTOR, test_keyboard_device_type)
{
	HIDReportDescriptor desc(keyboard_report_data, (uint16_t)sizeof(keyboard_report_data));
	uint16_t types = desc.get_device_types();
	EXPECT_TRUE(types & HID_DEVICE_KEYBOARD);
	EXPECT_FALSE(types & HID_DEVICE_MOUSE);
	EXPECT_FALSE(types & HID_DEVICE_JOYSTICK);
}

TEST(REPORT_DESCRIPTOR, test_no_joystick_in_mouse_descriptor)
{
	HIDReportDescriptor desc(mouse_report_data, (uint16_t)sizeof(mouse_report_data));
	uint16_t types = desc.get_device_types();
	EXPECT_EQ(types & (HID_DEVICE_JOYSTICK | HID_DEVICE_GAMEPAD), 0);
}
