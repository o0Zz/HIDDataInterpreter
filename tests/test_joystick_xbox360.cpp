#include <gtest/gtest.h>
#include "HIDReportDescriptor.h"
#include <HIDJoystick.h>

const uint8_t XBOX360_1[] = {
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
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(XBOX360_1, (uint16_t)sizeof(XBOX360_1));
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports().size(), 1);
}

TEST(XBOX360, test_report_is_gamepad)
{
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(XBOX360_1, (uint16_t)sizeof(XBOX360_1));
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->report_type, HIDReportType::GamePad);
}


TEST(XBOX360, test_report_correct_inputs)
{
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(XBOX360_1, (uint16_t)sizeof(XBOX360_1));
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs.size(), 24);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].type, (HIDInputType)0x3F);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[1].type, HIDInputType::ByteCount);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[2].type, HIDInputType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[3].type, HIDInputType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[4].type, HIDInputType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[5].type, HIDInputType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[6].type, HIDInputType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[7].type, HIDInputType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[8].type, HIDInputType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[9].type, HIDInputType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[10].type, HIDInputType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[11].type, HIDInputType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[12].type, HIDInputType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[13].type, HIDInputType::Padding);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[14].type, HIDInputType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[15].type, HIDInputType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[16].type, HIDInputType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[17].type, HIDInputType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[18].type, HIDInputType::Z);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[19].type, HIDInputType::Rz);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[20].type, HIDInputType::X);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[21].type, HIDInputType::Y);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[22].type, HIDInputType::Rx);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[23].type, HIDInputType::Ry);
}


TEST(XBOX360, test_input_parsing_button1)
{
	uint8_t data[] = {0x00, 0x14, 0x00, 0x10, 0x00, 0x00, 0xcd, 0x01, 0x36, 0xfd, 0x2c, 0x03, 0xe9, 0xfb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(XBOX360_1, (uint16_t)sizeof(XBOX360_1));

	HIDJoystickData joystick_data;
	HIDJoystick joystick(hid_report_descriptor);
	
	//GTEST_ASSERT_EQ(joystick.parseData(data, sizeof(data), &joystick_data), true);

	//GTEST_ASSERT_EQ(joystick_data.buttons[0], 1);
}
