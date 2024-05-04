#include <gtest/gtest.h>
#include "HIDReportDescriptor.h"
#include <HIDJoystick.h>

const uint8_t report_data[] = {
	0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
	0x09, 0x04,        // Usage (Joystick)
	0xA1, 0x01,        // Collection (Application)
	0x85, 0x02,        //   Report ID (2)
	0x09, 0x00,        //   Usage (Undefined)
	0xA1, 0x02,        //   Collection (Logical)
	0x09, 0x32,        //     Usage (Z)
	0x15, 0x00,        //     Logical Minimum (0)
	0x25, 0xFF,        //     Logical Maximum (-1)
	0x35, 0x00,        //     Physical Minimum (0)
	0x45, 0xFF,        //     Physical Maximum (-1)
	0x65, 0x00,        //     Unit (None)
	0x55, 0x00,        //     Unit Exponent (0)
	0x75, 0x08,        //     Report Size (8)
	0x95, 0x01,        //     Report Count (1)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x09, 0x35,        //     Usage (Rz)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x09, 0x30,        //     Usage (X)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x09, 0x31,        //     Usage (Y)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x09, 0x39,        //     Usage (Hat switch)
	0x25, 0x07,        //     Logical Maximum (7)
	0x46, 0x3B, 0x01,  //     Physical Maximum (315)
	0x65, 0x14,        //     Unit (System: English Rotation, Length: Centimeter)
	0x75, 0x04,        //     Report Size (4)
	0x81, 0x42,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
	0x05, 0x09,        //     Usage Page (Button)
	0x19, 0x01,        //     Usage Minimum (0x01)
	0x29, 0x0C,        //     Usage Maximum (0x0C)
	0x25, 0x01,        //     Logical Maximum (1)
	0x45, 0x01,        //     Physical Maximum (1)
	0x65, 0x00,        //     Unit (None)
	0x75, 0x01,        //     Report Size (1)
	0x95, 0x0C,        //     Report Count (12)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x95, 0x08,        //     Report Count (8)
	0x81, 0x03,        //     Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0xC1, 0x00,        //   End Collection
	0x06, 0x00, 0xFF,  //   Usage Page (Vendor Defined 0xFF00)
	0x09, 0x00,        //   Usage (0x00)
	0xA1, 0x02,        //   Collection (Logical)
	0x09, 0x02,        //     Usage (0x02)
	0x25, 0xFF,        //     Logical Maximum (-1)
	0x45, 0xFF,        //     Physical Maximum (-1)
	0x75, 0x08,        //     Report Size (8)
	0x95, 0x04,        //     Report Count (4)
	0x91, 0x02,        //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0xC1, 0x00,        //   End Collection
	0xC1, 0x00,        // End Collection
	};

TEST(PS1, test_report_has_1_joystick)
{
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data));
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports().size(), 1);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->report_type, HIDIOReportType::Joystick);
}

TEST(PS1, test_report_correct_inputs)
{
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data));
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data.size(), 26);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[0].type, HIDIOType::ReportId);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[1].type, HIDIOType::Z);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[2].type, HIDIOType::Rz);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[3].type, HIDIOType::X);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[4].type, HIDIOType::Y);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[5].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[6].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[7].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[8].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[9].type, HIDIOType::HatSwitch);
	// Buttons are ignored
	// Padding is ignored
}

TEST(PS1, test_input_parsing_incorrect_report_id)
{
	uint8_t data[] = {	
		0x01, 0x7f, 0x7f, 0x7f, 0x7f, 0x0f, 0x00, 0x00
		};

	HIDJoystick joystick(std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data)));
	HIDJoystickData joystick_data;
	
		//We should not be able to parse the data because reportID 1 is not present in report_data
	GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), false);
}

TEST(PS1, test_input_parsing_neutral)
{
	uint8_t data[] = {	
		0x02, 0x7f, 0x7f, 0x7f, 0x7f, 0x0f, 0x00, 0x00
		};

	HIDJoystick joystick(std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data)));
	HIDJoystickData joystick_data;
	
		//We should not be able to parse the data because reportID 1 is not present in report_data
	GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), true);
	GTEST_ASSERT_EQ(joystick_data.X, 127);
}