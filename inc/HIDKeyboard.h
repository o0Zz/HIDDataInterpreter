#pragma once

#include "HIDReportDescriptor.h"
#include <memory>

#define MAX_KEYBOARD_KEYS 6
#define MAX_KEYBOARD_MODIFIERS 8

#define KEYBOARD_MODIFIER_LEFT_CTRL    0x01
#define KEYBOARD_MODIFIER_LEFT_SHIFT   0x02
#define KEYBOARD_MODIFIER_LEFT_ALT     0x04
#define KEYBOARD_MODIFIER_LEFT_GUI     0x08
#define KEYBOARD_MODIFIER_RIGHT_CTRL   0x10
#define KEYBOARD_MODIFIER_RIGHT_SHIFT  0x20
#define KEYBOARD_MODIFIER_RIGHT_ALT    0x40
#define KEYBOARD_MODIFIER_RIGHT_GUI    0x80

class HIDKeyboardData
{
public:
    HIDKeyboardData();
    ~HIDKeyboardData();

    uint8_t modifiers; // bitmask of KEYBOARD_MODIFIER_* flags

    uint8_t key_count;
    uint8_t keys[MAX_KEYBOARD_KEYS]; // USB HID scan codes of currently pressed keys
};

class HIDKeyboard
{
public:
    HIDKeyboard(const std::shared_ptr<HIDReportDescriptor> &descriptor);
    ~HIDKeyboard();

    bool isValid();
    uint8_t getCount();

    bool parseData(uint8_t *data, uint16_t datalen, HIDKeyboardData *keyboard_data);

private:
    std::vector<HIDIOReport> m_reports;
};
