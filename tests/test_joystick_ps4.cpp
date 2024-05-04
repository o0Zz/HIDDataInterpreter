#include <gtest/gtest.h>
#include "HIDReportDescriptor.h"
#include <HIDJoystick.h>

const uint8_t report_data[] = {
		0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
		0x09, 0x05,        // Usage (Game Pad)
		0xA1, 0x01,        // Collection (Application)
		0x85, 0x01,        //   Report ID (1)
		0x09, 0x30,        //   Usage (X)
		0x09, 0x31,        //   Usage (Y)
		0x09, 0x32,        //   Usage (Z)
		0x09, 0x35,        //   Usage (Rz)
		0x15, 0x00,        //   Logical Minimum (0)
		0x26, 0xFF, 0x00,  //   Logical Maximum (255)
		0x75, 0x08,        //   Report Size (8)
		0x95, 0x04,        //   Report Count (4)
		0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
		0x09, 0x39,        //   Usage (Hat switch)
		0x15, 0x00,        //   Logical Minimum (0)
		0x25, 0x07,        //   Logical Maximum (7)
		0x35, 0x00,        //   Physical Minimum (0)
		0x46, 0x3B, 0x01,  //   Physical Maximum (315)
		0x65, 0x14,        //   Unit (System: English Rotation, Length: Centimeter)
		0x75, 0x04,        //   Report Size (4)
		0x95, 0x01,        //   Report Count (1)
		0x81, 0x42,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
		0x65, 0x00,        //   Unit (None)
		0x05, 0x09,        //   Usage Page (Button)
		0x19, 0x01,        //   Usage Minimum (0x01)
		0x29, 0x0E,        //   Usage Maximum (0x0E)
		0x15, 0x00,        //   Logical Minimum (0)
		0x25, 0x01,        //   Logical Maximum (1)
		0x75, 0x01,        //   Report Size (1)
		0x95, 0x0E,        //   Report Count (14)
		0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
		0x06, 0x00, 0xFF,  //   Usage Page (Vendor Defined 0xFF00)
		0x09, 0x20,        //   Usage (0x20)
		0x75, 0x06,        //   Report Size (6)
		0x95, 0x01,        //   Report Count (1)
		0x15, 0x00,        //   Logical Minimum (0)
		0x25, 0x7F,        //   Logical Maximum (127)
		0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
		0x05, 0x01,        //   Usage Page (Generic Desktop Ctrls)
		0x09, 0x33,        //   Usage (Rx)
		0x09, 0x34,        //   Usage (Ry)
		0x15, 0x00,        //   Logical Minimum (0)
		0x26, 0xFF, 0x00,  //   Logical Maximum (255)
		0x75, 0x08,        //   Report Size (8)
		0x95, 0x02,        //   Report Count (2)
		0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
		0x06, 0x00, 0xFF,  //   Usage Page (Vendor Defined 0xFF00)
		0x09, 0x21,        //   Usage (0x21)
		0x95, 0x36,        //   Report Count (54)
		0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
		0x85, 0x05,        //   Report ID (5)
		0x09, 0x22,        //   Usage (0x22)
		0x95, 0x1F,        //   Report Count (31)
		0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x04,        //   Report ID (4)
		0x09, 0x23,        //   Usage (0x23)
		0x95, 0x24,        //   Report Count (36)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x02,        //   Report ID (2)
		0x09, 0x24,        //   Usage (0x24)
		0x95, 0x24,        //   Report Count (36)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x08,        //   Report ID (8)
		0x09, 0x25,        //   Usage (0x25)
		0x95, 0x03,        //   Report Count (3)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x10,        //   Report ID (16)
		0x09, 0x26,        //   Usage (0x26)
		0x95, 0x04,        //   Report Count (4)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x11,        //   Report ID (17)
		0x09, 0x27,        //   Usage (0x27)
		0x95, 0x02,        //   Report Count (2)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x12,        //   Report ID (18)
		0x06, 0x02, 0xFF,  //   Usage Page (Vendor Defined 0xFF02)
		0x09, 0x21,        //   Usage (0x21)
		0x95, 0x0F,        //   Report Count (15)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x13,        //   Report ID (19)
		0x09, 0x22,        //   Usage (0x22)
		0x95, 0x16,        //   Report Count (22)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x14,        //   Report ID (20)
		0x06, 0x05, 0xFF,  //   Usage Page (Vendor Defined 0xFF05)
		0x09, 0x20,        //   Usage (0x20)
		0x95, 0x10,        //   Report Count (16)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x15,        //   Report ID (21)
		0x09, 0x21,        //   Usage (0x21)
		0x95, 0x2C,        //   Report Count (44)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x06, 0x80, 0xFF,  //   Usage Page (Vendor Defined 0xFF80)
		0x85, 0x80,        //   Report ID (-128)
		0x09, 0x20,        //   Usage (0x20)
		0x95, 0x06,        //   Report Count (6)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x81,        //   Report ID (-127)
		0x09, 0x21,        //   Usage (0x21)
		0x95, 0x06,        //   Report Count (6)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x82,        //   Report ID (-126)
		0x09, 0x22,        //   Usage (0x22)
		0x95, 0x05,        //   Report Count (5)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x83,        //   Report ID (-125)
		0x09, 0x23,        //   Usage (0x23)
		0x95, 0x01,        //   Report Count (1)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x84,        //   Report ID (-124)
		0x09, 0x24,        //   Usage (0x24)
		0x95, 0x04,        //   Report Count (4)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x85,        //   Report ID (-123)
		0x09, 0x25,        //   Usage (0x25)
		0x95, 0x06,        //   Report Count (6)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x86,        //   Report ID (-122)
		0x09, 0x26,        //   Usage (0x26)
		0x95, 0x06,        //   Report Count (6)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x87,        //   Report ID (-121)
		0x09, 0x27,        //   Usage (0x27)
		0x95, 0x23,        //   Report Count (35)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x88,        //   Report ID (-120)
		0x09, 0x28,        //   Usage (0x28)
		0x95, 0x3F,        //   Report Count (63)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x89,        //   Report ID (-119)
		0x09, 0x29,        //   Usage (0x29)
		0x95, 0x02,        //   Report Count (2)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x90,        //   Report ID (-112)
		0x09, 0x30,        //   Usage (0x30)
		0x95, 0x05,        //   Report Count (5)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x91,        //   Report ID (-111)
		0x09, 0x31,        //   Usage (0x31)
		0x95, 0x03,        //   Report Count (3)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x92,        //   Report ID (-110)
		0x09, 0x32,        //   Usage (0x32)
		0x95, 0x03,        //   Report Count (3)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x93,        //   Report ID (-109)
		0x09, 0x33,        //   Usage (0x33)
		0x95, 0x0C,        //   Report Count (12)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0x94,        //   Report ID (-108)
		0x09, 0x34,        //   Usage (0x34)
		0x95, 0x3F,        //   Report Count (63)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xA0,        //   Report ID (-96)
		0x09, 0x40,        //   Usage (0x40)
		0x95, 0x06,        //   Report Count (6)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xA1,        //   Report ID (-95)
		0x09, 0x41,        //   Usage (0x41)
		0x95, 0x01,        //   Report Count (1)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xA2,        //   Report ID (-94)
		0x09, 0x42,        //   Usage (0x42)
		0x95, 0x01,        //   Report Count (1)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xA3,        //   Report ID (-93)
		0x09, 0x43,        //   Usage (0x43)
		0x95, 0x30,        //   Report Count (48)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xA4,        //   Report ID (-92)
		0x09, 0x44,        //   Usage (0x44)
		0x95, 0x0D,        //   Report Count (13)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xF0,        //   Report ID (-16)
		0x09, 0x47,        //   Usage (0x47)
		0x95, 0x3F,        //   Report Count (63)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xF1,        //   Report ID (-15)
		0x09, 0x48,        //   Usage (0x48)
		0x95, 0x3F,        //   Report Count (63)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xF2,        //   Report ID (-14)
		0x09, 0x49,        //   Usage (0x49)
		0x95, 0x0F,        //   Report Count (15)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xA7,        //   Report ID (-89)
		0x09, 0x4A,        //   Usage (0x4A)
		0x95, 0x01,        //   Report Count (1)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xA8,        //   Report ID (-88)
		0x09, 0x4B,        //   Usage (0x4B)
		0x95, 0x01,        //   Report Count (1)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xA9,        //   Report ID (-87)
		0x09, 0x4C,        //   Usage (0x4C)
		0x95, 0x08,        //   Report Count (8)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xAA,        //   Report ID (-86)
		0x09, 0x4E,        //   Usage (0x4E)
		0x95, 0x01,        //   Report Count (1)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xAB,        //   Report ID (-85)
		0x09, 0x4F,        //   Usage (0x4F)
		0x95, 0x39,        //   Report Count (57)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xAC,        //   Report ID (-84)
		0x09, 0x50,        //   Usage (0x50)
		0x95, 0x39,        //   Report Count (57)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xAD,        //   Report ID (-83)
		0x09, 0x51,        //   Usage (0x51)
		0x95, 0x0B,        //   Report Count (11)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xAE,        //   Report ID (-82)
		0x09, 0x52,        //   Usage (0x52)
		0x95, 0x01,        //   Report Count (1)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xAF,        //   Report ID (-81)
		0x09, 0x53,        //   Usage (0x53)
		0x95, 0x02,        //   Report Count (2)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xB0,        //   Report ID (-80)
		0x09, 0x54,        //   Usage (0x54)
		0x95, 0x3F,        //   Report Count (63)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xE0,        //   Report ID (-32)
		0x09, 0x57,        //   Usage (0x57)
		0x95, 0x02,        //   Report Count (2)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xB3,        //   Report ID (-77)
		0x09, 0x55,        //   Usage (0x55)
		0x95, 0x3F,        //   Report Count (63)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xB4,        //   Report ID (-76)
		0x09, 0x55,        //   Usage (0x55)
		0x95, 0x3F,        //   Report Count (63)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xB5,        //   Report ID (-75)
		0x09, 0x56,        //   Usage (0x56)
		0x95, 0x3F,        //   Report Count (63)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xD0,        //   Report ID (-48)
		0x09, 0x58,        //   Usage (0x58)
		0x95, 0x3F,        //   Report Count (63)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0x85, 0xD4,        //   Report ID (-44)
		0x09, 0x59,        //   Usage (0x59)
		0x95, 0x3F,        //   Report Count (63)
		0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0xC0,              // End Collection
	};


TEST(PS4, test_report_has_1_gamepad)
{
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data));
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports().size(), 1);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->report_type, HIDIOReportType::GamePad);
}

TEST(PS4, test_report_correct_inputs)
{
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data));
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data.size(), 77);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[0].type, HIDIOType::ReportId);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[1].type, HIDIOType::X);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[2].type, HIDIOType::Y);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[3].type, HIDIOType::Z);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[4].type, HIDIOType::Rz);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[5].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[6].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[7].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[8].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[9].type, HIDIOType::HatSwitch);
	// Buttons are ignored
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[18].type, HIDIOType::VendorDefined);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[19].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[20].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[21].type, HIDIOType::Rx);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[22].type, HIDIOType::Ry);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[23].type, HIDIOType::VendorDefined);
	// Padding is ignored
}

TEST(PS4, test_report_correct_features)
{
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data));
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->features.size(), 48);
}

TEST(PS4, test_input_parsing_button1)
{
	uint8_t data[] = {	
		0x01, 0x83, 0x79, 0x84, 0x80, 0x18, 0x00, 0x50, 0x00, 0x00, 0xb5, 0xe0, 0xfe, 0xfa, 0xff, 
		0xfc, 0xff, 0xff, 0xff, 0x3e, 0xf2, 0x4b, 0x17, 0x0e, 0xef, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x16, 0x00, 0x00, 0x01, 0xc0, 0x81, 0xd3, 0x86, 0x02, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 
		0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 
		0x00, 0x00, 0x80, 0x00
		};

	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data));

	HIDJoystickData joystick_data;
	HIDJoystick joystick(hid_report_descriptor);
	
	GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), true);

	GTEST_ASSERT_EQ(joystick_data.buttons[1], 1);
}

TEST(PS4, test_input_parsing_left)
{
	uint8_t data[] = {	
		0x01, 0x00, 0x5b, 0x82, 0x7f, 0x08, 0x00, 0xcc, 0x00, 0x00, 0x17, 0x74, 0x07, 0xee, 0xff, 
		0x01, 0x00, 0xfa, 0xff, 0xc2, 0xef, 0x69, 0x1a, 0x46, 0xfa, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x16, 0x00, 0x00, 0x01, 0x10, 0x82, 0xb7, 0x01, 0x26, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 
		0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 
		0x00, 0x00, 0x80, 0x00
	};

	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data));

	HIDJoystickData joystick_data;
	HIDJoystick joystick(hid_report_descriptor);
	
	GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), true);

	GTEST_ASSERT_EQ(joystick_data.sticks[0].X, 0);
}

TEST(PS4, test_input_parsing_hatswitch_up)
{
	uint8_t data[] = {	
		0x01, 0x82, 0x7a, 0x82, 0x80, 0x00, 0x00, 0xbc, 0x00, 0x00, 0x72, 0x05, 0xff, 0xf2, 0xff, 
		0x01, 0x00, 0xf8, 0xff, 0x59, 0x01, 0x5b, 0x1f, 0xec, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x16, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 
		0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 
		0x00, 0x00, 0x80, 0x00
	};

	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data));

	HIDJoystickData joystick_data;
	HIDJoystick joystick(hid_report_descriptor);
	
	GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), true);

	GTEST_ASSERT_EQ(joystick_data.hat_switch, HIDJoystickHatSwitch::UP);
}

TEST(PS4, test_input_parsing_hatswitch_down)
{
	uint8_t data[] = {	
		0x01, 0x82, 0x7a, 0x82, 0x80, 0x04, 0x00, 0xbc, 0x00, 0x00, 0x72, 0x05, 0xff, 0xf2, 0xff, 
		0x01, 0x00, 0xf8, 0xff, 0x59, 0x01, 0x5b, 0x1f, 0xec, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x16, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 
		0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 
		0x00, 0x00, 0x80, 0x00
	};

	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data));

	HIDJoystickData joystick_data;
	HIDJoystick joystick(hid_report_descriptor);
	
	GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), true);

	GTEST_ASSERT_EQ(joystick_data.hat_switch, HIDJoystickHatSwitch::DOWN);
}

TEST(PS4, test_input_parsing_hatswitch_neutral)
{
	uint8_t data[] = {	
		0x01, 0x82, 0x7a, 0x82, 0x80, 0x08, 0x00, 0xbc, 0x00, 0x00, 0x72, 0x05, 0xff, 0xf2, 0xff, 
		0x01, 0x00, 0xf8, 0xff, 0x59, 0x01, 0x5b, 0x1f, 0xec, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x16, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 
		0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 
		0x00, 0x00, 0x80, 0x00
	};

	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data));

	HIDJoystickData joystick_data;
	HIDJoystick joystick(hid_report_descriptor);
	
	GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), true);

	GTEST_ASSERT_EQ(joystick_data.hat_switch, HIDJoystickHatSwitch::NEUTRAL);
}
