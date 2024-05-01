#include <gtest/gtest.h>
#include "HIDReportDescriptor.h"

const uint8_t report_data[] = {
	0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
	0x09, 0x02,        // Usage (Mouse)
	0xA1, 0x01,        // Collection (Application)
	0x09, 0x01,        //   Usage (Pointer)
	0xA1, 0x00,        //   Collection (Physical)
	0x05, 0x09,        //     Usage Page (Button)
	0x19, 0x01,        //     Usage Minimum (0x01)
	0x29, 0x05,        //     Usage Maximum (0x05)
	0x15, 0x00,        //     Logical Minimum (0)
	0x25, 0x01,        //     Logical Maximum (1)
	0x75, 0x01,        //     Report Size (1)
	0x95, 0x05,        //     Report Count (5)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x75, 0x01,        //     Report Size (1)
	0x95, 0x03,        //     Report Count (3)
	0x81, 0x03,        //     Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x06, 0x00, 0xFF,  //     Usage Page (Vendor Defined 0xFF00)
	0x09, 0x40,        //     Usage (0x40)
	0x75, 0x08,        //     Report Size (8)
	0x95, 0x02,        //     Report Count (2)
	0x15, 0x81,        //     Logical Minimum (-127)
	0x25, 0x7F,        //     Logical Maximum (127)
	0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
	0x09, 0x38,        //     Usage (Wheel)
	0x15, 0x81,        //     Logical Minimum (-127)
	0x25, 0x7F,        //     Logical Maximum (127)
	0x75, 0x08,        //     Report Size (8)
	0x95, 0x01,        //     Report Count (1)
	0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
	0x09, 0x30,        //     Usage (X)
	0x09, 0x31,        //     Usage (Y)
	0x16, 0x00, 0x80,  //     Logical Minimum (-32768)
	0x26, 0xFF, 0x7F,  //     Logical Maximum (32767)
	0x75, 0x10,        //     Report Size (16)
	0x95, 0x02,        //     Report Count (2)
	0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
	0xC0,              //   End Collection
	0x06, 0x00, 0xFF,  //   Usage Page (Vendor Defined 0xFF00)
	0x09, 0x02,        //   Usage (0x02)
	0x15, 0x00,        //   Logical Minimum (0)
	0x25, 0x01,        //   Logical Maximum (1)
	0x75, 0x08,        //   Report Size (8)
	0x95, 0x5A,        //   Report Count (90)
	0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0xC0,              // End Collection
};

TEST(MOUSE, test_report_has_1_mouse)
{
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data));
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports().size(), 1);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->report_type, HIDReportType::Mouse);
}

TEST(MOUSE, test_report_correct_inputs)
{
	std::shared_ptr<HIDReportDescriptor> hid_report_descriptor = std::make_shared<HIDReportDescriptor>(report_data, (uint16_t)sizeof(report_data));
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data.size(), 13);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[0].type, HIDIOType::Padding);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[7].type, HIDIOType::Button);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[8].type, HIDIOType::VendorDefined);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[10].type, HIDIOType::Wheel);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[11].type, HIDIOType::X);
	GTEST_ASSERT_EQ(hid_report_descriptor->GetReports()[0]->inputs[0].data[12].type, HIDIOType::Y);
}
