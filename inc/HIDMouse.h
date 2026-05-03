#pragma once

#include "HIDReportDescriptor.h"
#include <memory>

#define MAX_MOUSE_BUTTONS 8

#define MOUSE_SUPPORT_X      0x0001
#define MOUSE_SUPPORT_Y      0x0002
#define MOUSE_SUPPORT_WHEEL  0x0004

class HIDMouseData
{
public:
    HIDMouseData();
    ~HIDMouseData();

    uint16_t support;

    int16_t x;  // relative delta
    int16_t y;  // relative delta
    int16_t wheel; // relative delta

    uint8_t button_count;
    uint8_t buttons[MAX_MOUSE_BUTTONS];
};

class HIDMouse
{
public:
    HIDMouse(const std::shared_ptr<HIDReportDescriptor> &descriptor);
    ~HIDMouse();

    bool is_valid();
    uint8_t get_count();

    bool parse_data(uint8_t *data, uint16_t datalen, HIDMouseData *mouse_data);

private:
    std::vector<HIDIOReport> m_reports;
};
