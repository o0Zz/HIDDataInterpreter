#include <gtest/gtest.h>
#include "HIDReportDescriptor.h"
#include "HIDKeyboard.h"

// Standard USB HID boot protocol keyboard descriptor
const uint8_t keyboard_report_data[] = {
	0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
	0x09, 0x06,        // Usage (Keyboard)
	0xA1, 0x01,        // Collection (Application)
	0x05, 0x07,        //   Usage Page (Keyboard/Keypad)
	0x19, 0xE0,        //   Usage Minimum (0xE0) Left Control
	0x29, 0xE7,        //   Usage Maximum (0xE7) Right GUI
	0x15, 0x00,        //   Logical Minimum (0)
	0x25, 0x01,        //   Logical Maximum (1)
	0x75, 0x01,        //   Report Size (1)
	0x95, 0x08,        //   Report Count (8)
	0x81, 0x02,        //   Input (Data,Var,Abs) - Modifier byte
	0x95, 0x01,        //   Report Count (1)
	0x75, 0x08,        //   Report Size (8)
	0x81, 0x03,        //   Input (Const,Var,Abs) - Reserved byte
	0x95, 0x06,        //   Report Count (6)
	0x75, 0x08,        //   Report Size (8)
	0x15, 0x00,        //   Logical Minimum (0)
	0x25, 0x65,        //   Logical Maximum (101)
	0x05, 0x07,        //   Usage Page (Keyboard/Keypad)
	0x19, 0x00,        //   Usage Minimum (0x00)
	0x29, 0x65,        //   Usage Maximum (0x65)
	0x81, 0x00,        //   Input (Data,Array,Abs) - Key array (6 keys)
	0xC0,              // End Collection
};

TEST(KEYBOARD, test_report_has_1_keyboard)
{
	HIDReportDescriptor hid_report_descriptor(keyboard_report_data, (uint16_t)sizeof(keyboard_report_data));
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports().size(), 1);
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].report_type, HIDIOReportType::Keyboard);
}

TEST(KEYBOARD, test_keyboard_is_valid)
{
	std::shared_ptr<HIDReportDescriptor> desc = std::make_shared<HIDReportDescriptor>(keyboard_report_data, (uint16_t)sizeof(keyboard_report_data));
	HIDKeyboard keyboard(desc);
	GTEST_ASSERT_EQ(keyboard.isValid(), true);
	GTEST_ASSERT_EQ(keyboard.getCount(), 1);
}

TEST(KEYBOARD, test_keyboard_parse_modifiers)
{
	std::shared_ptr<HIDReportDescriptor> desc = std::make_shared<HIDReportDescriptor>(keyboard_report_data, (uint16_t)sizeof(keyboard_report_data));
	HIDKeyboard keyboard(desc);
	HIDKeyboardData keyboard_data;

	// Standard boot protocol: byte 0 = modifiers, byte 1 = reserved, bytes 2-7 = keys
	// Left Ctrl (bit 0) + Left Shift (bit 1) = 0x03
	uint8_t data[] = {0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	GTEST_ASSERT_EQ(keyboard.parseData(data, sizeof(data), &keyboard_data), true);
	GTEST_ASSERT_EQ(keyboard_data.modifiers & KEYBOARD_MODIFIER_LEFT_CTRL, KEYBOARD_MODIFIER_LEFT_CTRL);
	GTEST_ASSERT_EQ(keyboard_data.modifiers & KEYBOARD_MODIFIER_LEFT_SHIFT, KEYBOARD_MODIFIER_LEFT_SHIFT);
	GTEST_ASSERT_EQ(keyboard_data.modifiers & KEYBOARD_MODIFIER_LEFT_ALT, 0);
}

TEST(KEYBOARD, test_keyboard_parse_keys)
{
	std::shared_ptr<HIDReportDescriptor> desc = std::make_shared<HIDReportDescriptor>(keyboard_report_data, (uint16_t)sizeof(keyboard_report_data));
	HIDKeyboard keyboard(desc);
	HIDKeyboardData keyboard_data;

	// Press 'a' (0x04) and 'b' (0x05)
	uint8_t data[] = {0x00, 0x00, 0x04, 0x05, 0x00, 0x00, 0x00, 0x00};
	GTEST_ASSERT_EQ(keyboard.parseData(data, sizeof(data), &keyboard_data), true);
	GTEST_ASSERT_EQ(keyboard_data.key_count, 2);
	GTEST_ASSERT_EQ(keyboard_data.keys[0], 0x04); // 'a'
	GTEST_ASSERT_EQ(keyboard_data.keys[1], 0x05); // 'b'
}

TEST(KEYBOARD, test_keyboard_parse_modifiers_and_keys)
{
	std::shared_ptr<HIDReportDescriptor> desc = std::make_shared<HIDReportDescriptor>(keyboard_report_data, (uint16_t)sizeof(keyboard_report_data));
	HIDKeyboard keyboard(desc);
	HIDKeyboardData keyboard_data;

	// Left Shift + 'a' (Shift+A)
	uint8_t data[] = {0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00};
	GTEST_ASSERT_EQ(keyboard.parseData(data, sizeof(data), &keyboard_data), true);
	GTEST_ASSERT_EQ(keyboard_data.modifiers & KEYBOARD_MODIFIER_LEFT_SHIFT, KEYBOARD_MODIFIER_LEFT_SHIFT);
	GTEST_ASSERT_EQ(keyboard_data.key_count, 1);
	GTEST_ASSERT_EQ(keyboard_data.keys[0], 0x04); // 'a'
}
