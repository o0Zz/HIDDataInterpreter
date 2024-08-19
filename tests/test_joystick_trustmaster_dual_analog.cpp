#include <gtest/gtest.h>
#include "HIDReportDescriptor.h"
#include "HIDJoystick.h"

const uint8_t report_data[] = {
		0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
		0x09, 0x05,        // Usage (Game Pad)
		0xA1, 0x01,        // Collection (Application)
		0x05, 0x01,        //   Usage Page (Generic Desktop Ctrls)
		0x09, 0x01,        //   Usage (Pointer)
		0xA1, 0x00,        //   Collection (Physical)
		0x05, 0x09,        //     Usage Page (Button)
		0x19, 0x01,        //     Usage Minimum (0x01)
		0x29, 0x0C,        //     Usage Maximum (0x0C)
		0x15, 0x00,        //     Logical Minimum (0)
		0x25, 0x01,        //     Logical Maximum (1)
		0x75, 0x01,        //     Report Size (1)
		0x95, 0x0C,        //     Report Count (12)
		0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
		0x75, 0x08,        //     Report Size (8)
		0x95, 0x01,        //     Report Count (1)
		0x81, 0x01,        //     Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
		0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
		0x09, 0x39,        //     Usage (Hat switch)
		0x25, 0x07,        //     Logical Maximum (7)
		0x35, 0x00,        //     Physical Minimum (0)
		0x46, 0x0E, 0x01,  //     Physical Maximum (270)
		0x66, 0x40, 0x00,  //     Unit (Length: Degrees)
		0x75, 0x04,        //     Report Size (4)
		0x81, 0x42,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,Null State)
		0x09, 0x30,        //     Usage (X)
		0x09, 0x31,        //     Usage (Y)
		0x15, 0x80,        //     Logical Minimum (-128)
		0x25, 0x7F,        //     Logical Maximum (127)
		0x46, 0xFF, 0x00,  //     Physical Maximum (255)
		0x66, 0x00, 0x00,  //     Unit (None)
		0x75, 0x08,        //     Report Size (8)
		0x95, 0x02,        //     Report Count (2)
		0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
		0x09, 0x35,        //     Usage (Rz)
		0x95, 0x01,        //     Report Count (1)
		0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
		0x09, 0x36,        //     Usage (Slider)
		0x16, 0x00, 0x00,  //     Logical Minimum (0)
		0x26, 0xFF, 0x00,  //     Logical Maximum (255)
		0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
		0x09, 0xBB,        //     Usage (0xBB)
		0x15, 0x00,        //     Logical Minimum (0)
		0x26, 0xFF, 0x00,  //     Logical Maximum (255)
		0x35, 0x00,        //     Physical Minimum (0)
		0x46, 0xFF, 0x00,  //     Physical Maximum (255)
		0x75, 0x08,        //     Report Size (8)
		0x95, 0x04,        //     Report Count (4)
		0x91, 0x02,        //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
		0xC0,              //   End Collection
		0xC0,              // End Collection
	};


TEST(TRUSTMASTER, test_report_has_1_gamepad)
{
	HIDReportDescriptor hid_report_descriptor(report_data, (uint16_t)sizeof(report_data));
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports().size(), 1);
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].report_type, HIDIOReportType::GamePad);
}

TEST(TRUSTMASTER, test_report_correct_inputs)
{
	HIDReportDescriptor hid_report_descriptor(report_data, (uint16_t)sizeof(report_data));
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].inputs[0].data.size(), 18);
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].inputs[0].data[0].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].inputs[0].data[1].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].inputs[0].data[2].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].inputs[0].data[3].type, HIDIOType::Button);
	//...
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].inputs[0].data[13].type, HIDIOType::HatSwitch);
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].inputs[0].data[14].type, HIDIOType::X);
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].inputs[0].data[15].type, HIDIOType::Y);
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].inputs[0].data[16].type, HIDIOType::Rz);
	GTEST_ASSERT_EQ(hid_report_descriptor.GetReports()[0].inputs[0].data[17].type, HIDIOType::Slider);
}

TEST(TRUSTMASTER, test_input_parsing_slider)
{
	uint8_t data[64] = {	
		//                X,    Y,    Rz,   Slider								
		0x00, 0x00, 0xF0, 0xDE, 0x00, 0x00, 0x80 
		};

	HIDJoystick joystick(std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data)));
	HIDJoystickData joystick_data;
	
	GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), true);

	GTEST_ASSERT_EQ(joystick_data.Y, 128);
	GTEST_ASSERT_EQ(joystick_data.Rz, 128);
	GTEST_ASSERT_EQ(joystick_data.Slider, 128);
}
