#include <gtest/gtest.h>
#include "HIDReportDescriptor.h"
#include "HIDJoystick.h"

uint8_t double_joystick_hid[] = 
{ //XM-10
	0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
	0x09, 0x04,        // Usage (Joystick)
	0xA1, 0x01,        // Collection (Application)
	0x05, 0x01,        //   Usage Page (Generic Desktop Ctrls)
	0x09, 0x01,        //   Usage (Pointer)
	0xA1, 0x00,        //   Collection (Physical)
	0x85, 0x01,        //     Report ID (1)
	0x09, 0x30,        //     Usage (X)
	0x09, 0x31,        //     Usage (Y)
	0x15, 0x00,        //     Logical Minimum (0)
	0x26, 0xFF, 0x00,  //     Logical Maximum (255)
	0x75, 0x08,        //     Report Size (8)
	0x95, 0x02,        //     Report Count (2)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0xC0,              //   End Collection
	0x05, 0x09,        //   Usage Page (Button)
	0x19, 0x01,        //   Usage Minimum (0x01)
	0x29, 0x0F,        //   Usage Maximum (0x0F)
	0x15, 0x00,        //   Logical Minimum (0)
	0x25, 0x01,        //   Logical Maximum (1)
	0x75, 0x01,        //   Report Size (1)
	0x95, 0x0F,        //   Report Count (15)
	0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x75, 0x01,        //   Report Size (1)
	0x95, 0x01,        //   Report Count (1)
	0x81, 0x03,        //   Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x05, 0x01,        //   Usage Page (Generic Desktop Ctrls)
	0x09, 0x39,        //   Usage (Hat switch)
	0x15, 0x00,        //   Logical Minimum (0)
	0x25, 0x07,        //   Logical Maximum (7)
	0x35, 0x00,        //   Physical Minimum (0)
	0x46, 0x3B, 0x01,  //   Physical Maximum (315)
	0x65, 0x14,        //   Unit (System: English Rotation, Length: Centimeter)
	0x75, 0x04,        //   Report Size (4)
	0x95, 0x01,        //   Report Count (1)
	0x81, 0x42,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
	0x55, 0x00,        //   Unit Exponent (0)
	0x65, 0x00,        //   Unit (None)
	0x81, 0x03,        //   Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0xC0,              // End Collection
	0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
	0x09, 0x04,        // Usage (Joystick)
	0xA1, 0x01,        // Collection (Application)
	0x05, 0x01,        //   Usage Page (Generic Desktop Ctrls)
	0x09, 0x01,        //   Usage (Pointer)
	0xA1, 0x00,        //   Collection (Physical)
	0x85, 0x02,        //     Report ID (2)
	0x09, 0x30,        //     Usage (X)
	0x09, 0x31,        //     Usage (Y)
	0x15, 0x00,        //     Logical Minimum (0)
	0x26, 0xFF, 0x00,  //     Logical Maximum (255)
	0x75, 0x08,        //     Report Size (8)
	0x95, 0x02,        //     Report Count (2)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0xC0,              //   End Collection
	0x05, 0x09,        //   Usage Page (Button)
	0x19, 0x01,        //   Usage Minimum (0x01)
	0x29, 0x0D,        //   Usage Maximum (0x0D)
	0x15, 0x00,        //   Logical Minimum (0)
	0x25, 0x01,        //   Logical Maximum (1)
	0x75, 0x01,        //   Report Size (1)
	0x95, 0x0D,        //   Report Count (13)
	0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x75, 0x01,        //   Report Size (1)
	0x95, 0x03,        //   Report Count (3)
	0x81, 0x03,        //   Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x05, 0x01,        //   Usage Page (Generic Desktop Ctrls)
	0x09, 0x39,        //   Usage (Hat switch)
	0x15, 0x00,        //   Logical Minimum (0)
	0x25, 0x07,        //   Logical Maximum (7)
	0x35, 0x00,        //   Physical Minimum (0)
	0x46, 0x3B, 0x01,  //   Physical Maximum (315)
	0x65, 0x14,        //   Unit (System: English Rotation, Length: Centimeter)
	0x75, 0x04,        //   Report Size (4)
	0x95, 0x01,        //   Report Count (1)
	0x81, 0x42,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
	0x55, 0x00,        //   Unit Exponent (0)
	0x65, 0x00,        //   Unit (None)
	0x81, 0x03,        //   Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0xC0,              // End Collection
	// 160 bytes
};

TEST(HID, test_report_has_2_joystick)
{
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(double_joystick_hid, (uint16_t)sizeof(double_joystick_hid));
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports().size(), 2);
}

TEST(HID, test_report_is_joystick)
{
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(double_joystick_hid, (uint16_t)sizeof(double_joystick_hid));
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->report_type, HIDReportType::Joystick);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[1]->report_type, HIDReportType::Joystick);
}

TEST(HID, test_report_ids_for_joysticks)
{
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(double_joystick_hid, (uint16_t)sizeof(double_joystick_hid));
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->report_id, 1);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[1]->report_id, 2);
}

TEST(HID, test_report_has_correct_inputs)
{
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(double_joystick_hid, (uint16_t)sizeof(double_joystick_hid));
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs.size(), 6);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].type, HIDInputType::X);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[1].type, HIDInputType::Y);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[2].type, HIDInputType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[3].type, HIDInputType::Padding);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[4].type, HIDInputType::HatSwitch);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[5].type, HIDInputType::Padding);
}


TEST(HID, test_input_parsing_button1)
{
	uint8_t data[] = {0x00, 0x14, 0x00, 0x10, 0x00, 0x00, 0xcd, 0x01, 0x36, 0xfd, 0x2c, 0x03, 0xe9, 0xfb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(double_joystick_hid, (uint16_t)sizeof(double_joystick_hid));

	HIDJoystickData joystick_data;
	HIDJoystick joystick(hid_report_descriptor);
	
	GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), true);

	GTEST_ASSERT_EQ(joystick_data.buttons[0], 1);
}
