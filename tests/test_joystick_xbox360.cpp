#include <gtest/gtest.h>
#include "HIDReportDescriptor.h"
#include <HIDJoystick.h>

const uint8_t report_data[] = {
	0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
	0x09, 0x05,        // Usage (Game Pad)
	0xA1, 0x01,        // Collection (Application)
	0x05, 0x01,        //   Usage Page (Generic Desktop Ctrls)
	0x09, 0x3A,        //   Usage (Counted Buffer)
	0xA1, 0x02,        //   Collection (Logical)
	0x75, 0x08,        //     Report Size (8)
	0x95, 0x02,        //     Report Count (2)
	0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
	0x09, 0x3F,        //     Usage (0x3F)
	0x09, 0x3B,        //     Usage (Byte Count)
	0x81, 0x01,        //     Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x75, 0x01,        //     Report Size (1)
	0x15, 0x00,        //     Logical Minimum (0)
	0x25, 0x01,        //     Logical Maximum (1)
	0x35, 0x00,        //     Physical Minimum (0)
	0x45, 0x01,        //     Physical Maximum (1)
	0x95, 0x04,        //     Report Count (4)
	0x05, 0x09,        //     Usage Page (Button)
	0x19, 0x0C,        //     Usage Minimum (0x0C)
	0x29, 0x0F,        //     Usage Maximum (0x0F)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x75, 0x01,        //     Report Size (1)
	0x15, 0x00,        //     Logical Minimum (0)
	0x25, 0x01,        //     Logical Maximum (1)
	0x35, 0x00,        //     Physical Minimum (0)
	0x45, 0x01,        //     Physical Maximum (1)
	0x95, 0x04,        //     Report Count (4)
	0x05, 0x09,        //     Usage Page (Button)
	0x09, 0x09,        //     Usage (0x09)
	0x09, 0x0A,        //     Usage (0x0A)
	0x09, 0x07,        //     Usage (0x07)
	0x09, 0x08,        //     Usage (0x08)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x75, 0x01,        //     Report Size (1)
	0x15, 0x00,        //     Logical Minimum (0)
	0x25, 0x01,        //     Logical Maximum (1)
	0x35, 0x00,        //     Physical Minimum (0)
	0x45, 0x01,        //     Physical Maximum (1)
	0x95, 0x03,        //     Report Count (3)
	0x05, 0x09,        //     Usage Page (Button)
	0x09, 0x05,        //     Usage (0x05)
	0x09, 0x06,        //     Usage (0x06)
	0x09, 0x0B,        //     Usage (0x0B)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x75, 0x01,        //     Report Size (1)
	0x95, 0x01,        //     Report Count (1)
	0x81, 0x01,        //     Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x75, 0x01,        //     Report Size (1)
	0x15, 0x00,        //     Logical Minimum (0)
	0x25, 0x01,        //     Logical Maximum (1)
	0x35, 0x00,        //     Physical Minimum (0)
	0x45, 0x01,        //     Physical Maximum (1)
	0x95, 0x04,        //     Report Count (4)
	0x05, 0x09,        //     Usage Page (Button)
	0x19, 0x01,        //     Usage Minimum (0x01)
	0x29, 0x04,        //     Usage Maximum (0x04)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x75, 0x08,        //     Report Size (8)
	0x15, 0x00,        //     Logical Minimum (0)
	0x26, 0xFF, 0x00,  //     Logical Maximum (255)
	0x35, 0x00,        //     Physical Minimum (0)
	0x46, 0xFF, 0x00,  //     Physical Maximum (255)
	0x95, 0x02,        //     Report Count (2)
	0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
	0x09, 0x32,        //     Usage (Z)
	0x09, 0x35,        //     Usage (Rz)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x75, 0x10,        //     Report Size (16)
	0x16, 0x00, 0x80,  //     Logical Minimum (-32768)
	0x26, 0xFF, 0x7F,  //     Logical Maximum (32767)
	0x36, 0x00, 0x80,  //     Physical Minimum (-32768)
	0x46, 0xFF, 0x7F,  //     Physical Maximum (32767)
	0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
	0x09, 0x01,        //     Usage (Pointer)
	0xA1, 0x00,        //     Collection (Physical)
	0x95, 0x02,        //       Report Count (2)
	0x05, 0x01,        //       Usage Page (Generic Desktop Ctrls)
	0x09, 0x30,        //       Usage (X)
	0x09, 0x31,        //       Usage (Y)
	0x81, 0x02,        //       Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0xC0,              //     End Collection
	0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
	0x09, 0x01,        //     Usage (Pointer)
	0xA1, 0x00,        //     Collection (Physical)
	0x95, 0x02,        //       Report Count (2)
	0x05, 0x01,        //       Usage Page (Generic Desktop Ctrls)
	0x09, 0x33,        //       Usage (Rx)
	0x09, 0x34,        //       Usage (Ry)
	0x81, 0x02,        //       Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0xC0,              //     End Collection
	0xC0,              //   End Collection
	0xC0,              // End Collection
};

TEST(XBOX360, test_report_has_1_gamepad)
{
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data));
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports().size(), 1);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->report_type, HIDIOReportType::GamePad);
}

TEST(XBOX360, test_report_correct_inputs)
{
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data));
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data.size(), 24);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[0].type, HIDIOType::Unknown);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[1].type, HIDIOType::Unknown);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[2].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[3].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[4].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[5].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[6].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[7].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[8].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[9].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[10].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[11].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[12].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[13].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[14].type, HIDIOType::Padding);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[15].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[16].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[17].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[18].type, HIDIOType::Z);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[19].type, HIDIOType::Rz);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[20].type, HIDIOType::X);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[21].type, HIDIOType::Y);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[22].type, HIDIOType::Rx);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[23].type, HIDIOType::Ry);
}


TEST(XBOX360, test_input_parsing_button1)
{
	uint8_t data[] = {0x00, 0x14, 0x00, 0x10, 0x00, 0x00, 0xcd, 0x01, 0x36, 0xfd, 0x2c, 0x03, 0xe9, 0xfb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data));

	HIDJoystickData joystick_data;
	HIDJoystick joystick(hid_report_descriptor);
	
	GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), true);

	GTEST_ASSERT_EQ(joystick_data.buttons[1], 1);
}
