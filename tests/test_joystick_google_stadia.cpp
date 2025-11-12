#include <gtest/gtest.h>
#include "HIDReportDescriptor.h"
#include "HIDJoystick.h"

const uint8_t report_data[] = {
	0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
	0x09, 0x05,        // Usage (Game Pad)
	0xA1, 0x01,        // Collection (Application)
	0x85, 0x03,        //   Report ID (3)
	0x05, 0x01,        //   Usage Page (Generic Desktop Ctrls)
	0x75, 0x04,        //   Report Size (4)
	0x95, 0x01,        //   Report Count (1)
	0x25, 0x07,        //   Logical Maximum (7)
	0x46, 0x3B, 0x01,  //   Physical Maximum (315)
	0x65, 0x14,        //   Unit (System: English Rotation, Length: Centimeter)
	0x09, 0x39,        //   Usage (Hat switch)
	0x81, 0x42,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
	0x45, 0x00,        //   Physical Maximum (0)
	0x65, 0x00,        //   Unit (None)
	0x75, 0x01,        //   Report Size (1)
	0x95, 0x04,        //   Report Count (4)
	0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x05, 0x09,        //   Usage Page (Button)
	0x15, 0x00,        //   Logical Minimum (0)
	0x25, 0x01,        //   Logical Maximum (1)
	0x75, 0x01,        //   Report Size (1)
	0x95, 0x0F,        //   Report Count (15)
	0x09, 0x12,        //   Usage (0x12)
	0x09, 0x11,        //   Usage (0x11)
	0x09, 0x14,        //   Usage (0x14)
	0x09, 0x13,        //   Usage (0x13)
	0x09, 0x0D,        //   Usage (0x0D)
	0x09, 0x0C,        //   Usage (0x0C)
	0x09, 0x0B,        //   Usage (0x0B)
	0x09, 0x0F,        //   Usage (0x0F)
	0x09, 0x0E,        //   Usage (0x0E)
	0x09, 0x08,        //   Usage (0x08)
	0x09, 0x07,        //   Usage (0x07)
	0x09, 0x05,        //   Usage (0x05)
	0x09, 0x04,        //   Usage (0x04)
	0x09, 0x02,        //   Usage (0x02)
	0x09, 0x01,        //   Usage (0x01)
	0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x75, 0x01,        //   Report Size (1)
	0x95, 0x01,        //   Report Count (1)
	0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x05, 0x01,        //   Usage Page (Generic Desktop Ctrls)
	0x15, 0x01,        //   Logical Minimum (1)
	0x26, 0xFF, 0x00,  //   Logical Maximum (255)
	0x09, 0x01,        //   Usage (Pointer)
	0xA1, 0x00,        //   Collection (Physical)
	0x09, 0x30,        //     Usage (X)
	0x09, 0x31,        //     Usage (Y)
	0x75, 0x08,        //     Report Size (8)
	0x95, 0x02,        //     Report Count (2)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0xC0,              //   End Collection
	0x09, 0x01,        //   Usage (Pointer)
	0xA1, 0x00,        //   Collection (Physical)
	0x09, 0x32,        //     Usage (Z)
	0x09, 0x35,        //     Usage (Rz)
	0x75, 0x08,        //     Report Size (8)
	0x95, 0x02,        //     Report Count (2)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0xC0,              //   End Collection
	0x05, 0x02,        //   Usage Page (Sim Ctrls)
	0x75, 0x08,        //   Report Size (8)
	0x95, 0x02,        //   Report Count (2)
	0x15, 0x00,        //   Logical Minimum (0)
	0x26, 0xFF, 0x00,  //   Logical Maximum (255)
	0x09, 0xC5,        //   Usage (Brake)
	0x09, 0xC4,        //   Usage (Accelerator)
	0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x05, 0x0C,        //   Usage Page (Consumer)
	0x15, 0x00,        //   Logical Minimum (0)
	0x25, 0x01,        //   Logical Maximum (1)
	0x09, 0xE9,        //   Usage (Volume Increment)
	0x09, 0xEA,        //   Usage (Volume Decrement)
	0x75, 0x01,        //   Report Size (1)
	0x95, 0x02,        //   Report Count (2)
	0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x09, 0xCD,        //   Usage (Play/Pause)
	0x95, 0x01,        //   Report Count (1)
	0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x95, 0x05,        //   Report Count (5)
	0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x85, 0x05,        //   Report ID (5)
	0x06, 0x0F, 0x00,  //   Usage Page (PID Page)
	0x09, 0x97,        //   Usage (0x97)
	0x75, 0x10,        //   Report Size (16)
	0x95, 0x02,        //   Report Count (2)
	0x27, 0xFF, 0xFF, 0x00, 0x00,  //   Logical Maximum (65534)
	0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0xC0,              // End Collection
	};


TEST(GoogleStadia, test_report_has_1_gamepad)
{
	HIDReportDescriptor hid_report_descriptor(report_data, (uint16_t)sizeof(report_data));
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports().size(), 1);
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].report_type, HIDIOReportType::GamePad);
}

TEST(GoogleStadia, test_report_correct_inputs)
{
	HIDReportDescriptor hid_report_descriptor(report_data, (uint16_t)sizeof(report_data));
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].inputs[0].data.size(), 36);
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].inputs[0].data[0].type, HIDIOType::ReportId);
	//...
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].inputs[0].data[26].type, HIDIOType::Brake);
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].inputs[0].data[27].type, HIDIOType::Accelerator);
}

TEST(GoogleStadia, test_input_parsing_accelerator)
{
	uint8_t data[] = {	
		0x03, 0x08, 0x08, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0xFC, 0x00
		};

	HIDJoystick joystick(std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data)));
	HIDJoystickData joystick_data;
	
	GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), true);

	GTEST_ASSERT_EQ(joystick_data.Brake, -32768);
	GTEST_ASSERT_EQ(joystick_data.Accelerator, 31996);
}

TEST(GoogleStadia, test_input_parsing_play_vol)
{
	uint8_t data[] = {	
		0x03, 0x08, 0x08, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0xFF
		};

	HIDJoystick joystick(std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data)));
	HIDJoystickData joystick_data;
	
	GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), true);

	GTEST_ASSERT_EQ(joystick_data.consumer_buttons[HIDIOConsumerType::VolumeIncrement], 1);
	GTEST_ASSERT_EQ(joystick_data.consumer_buttons[HIDIOConsumerType::VolumeDecrement], 1);
	GTEST_ASSERT_EQ(joystick_data.consumer_buttons[HIDIOConsumerType::PlayPause], 1);
	GTEST_ASSERT_EQ(joystick_data.consumer_buttons[HIDIOConsumerType::Menu], 0);
}
