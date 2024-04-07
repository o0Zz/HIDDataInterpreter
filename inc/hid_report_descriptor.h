#pragma once
#include <stdint.h>

class HIDReportDescriptor
{
    HIDReportDescriptor(uint_8_t *hid_report_data);
    ~HIDReportDescriptor();
};
