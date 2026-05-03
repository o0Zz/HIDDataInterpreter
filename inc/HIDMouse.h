#pragma once

#include "HIDReportDescriptor.h"
#include <memory>

#define MAX_MOUSE_BUTTONS 8

#define MOUSE_SUPPORT_X      0x0001
#define MOUSE_SUPPORT_Y      0x0002
#define MOUSE_SUPPORT_Wheel  0x0004

class HIDMouseData
{
public:
    HIDMouseData();
    ~HIDMouseData();

    uint16_t support;

    int16_t X;  // relative delta
    int16_t Y;  // relative delta
    int16_t Wheel; // relative delta

    uint8_t button_count;
    uint8_t buttons[MAX_MOUSE_BUTTONS];
};

class HIDMouse
{
public:
    HIDMouse(const std::shared_ptr<HIDReportDescriptor> &descriptor);
    ~HIDMouse();

    bool isValid();
    uint8_t getCount();

    bool parseData(uint8_t *data, uint16_t datalen, HIDMouseData *mouse_data);

private:
    std::vector<HIDIOReport> m_reports;
};
