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

// USB HID Keyboard/Keypad Usage Page (0x07) scan codes
enum class HIDKeyboardKey : uint8_t
{
    None              = 0x00,
    ErrorRollOver     = 0x01,
    POSTFail          = 0x02,
    ErrorUndefined    = 0x03,
    A                 = 0x04,
    B                 = 0x05,
    C                 = 0x06,
    D                 = 0x07,
    E                 = 0x08,
    F                 = 0x09,
    G                 = 0x0A,
    H                 = 0x0B,
    I                 = 0x0C,
    J                 = 0x0D,
    K                 = 0x0E,
    L                 = 0x0F,
    M                 = 0x10,
    N                 = 0x11,
    O                 = 0x12,
    P                 = 0x13,
    Q                 = 0x14,
    R                 = 0x15,
    S                 = 0x16,
    T                 = 0x17,
    U                 = 0x18,
    V                 = 0x19,
    W                 = 0x1A,
    X                 = 0x1B,
    Y                 = 0x1C,
    Z                 = 0x1D,
    Num1              = 0x1E,
    Num2              = 0x1F,
    Num3              = 0x20,
    Num4              = 0x21,
    Num5              = 0x22,
    Num6              = 0x23,
    Num7              = 0x24,
    Num8              = 0x25,
    Num9              = 0x26,
    Num0              = 0x27,
    Enter             = 0x28,
    Escape            = 0x29,
    Backspace         = 0x2A,
    Tab               = 0x2B,
    Space             = 0x2C,
    Minus             = 0x2D,
    Equal             = 0x2E,
    LeftBracket       = 0x2F,
    RightBracket      = 0x30,
    Backslash         = 0x31,
    NonUSHash         = 0x32,
    Semicolon         = 0x33,
    Apostrophe        = 0x34,
    Grave             = 0x35,
    Comma             = 0x36,
    Period            = 0x37,
    Slash             = 0x38,
    CapsLock          = 0x39,
    F1                = 0x3A,
    F2                = 0x3B,
    F3                = 0x3C,
    F4                = 0x3D,
    F5                = 0x3E,
    F6                = 0x3F,
    F7                = 0x40,
    F8                = 0x41,
    F9                = 0x42,
    F10               = 0x43,
    F11               = 0x44,
    F12               = 0x45,
    PrintScreen       = 0x46,
    ScrollLock        = 0x47,
    Pause             = 0x48,
    Insert            = 0x49,
    Home              = 0x4A,
    PageUp            = 0x4B,
    Delete            = 0x4C,
    End               = 0x4D,
    PageDown          = 0x4E,
    RightArrow        = 0x4F,
    LeftArrow         = 0x50,
    DownArrow         = 0x51,
    UpArrow           = 0x52,
    NumLock           = 0x53,
    KeypadDivide      = 0x54,
    KeypadMultiply    = 0x55,
    KeypadSubtract    = 0x56,
    KeypadAdd         = 0x57,
    KeypadEnter       = 0x58,
    Keypad1           = 0x59,
    Keypad2           = 0x5A,
    Keypad3           = 0x5B,
    Keypad4           = 0x5C,
    Keypad5           = 0x5D,
    Keypad6           = 0x5E,
    Keypad7           = 0x5F,
    Keypad8           = 0x60,
    Keypad9           = 0x61,
    Keypad0           = 0x62,
    KeypadDecimal     = 0x63,
    NonUSBackslash    = 0x64,
    Application       = 0x65,
};

class HIDKeyboardData
{
public:
    HIDKeyboardData();
    ~HIDKeyboardData();

    uint8_t modifiers; // bitmask of KEYBOARD_MODIFIER_* flags

    uint8_t key_count;
    HIDKeyboardKey keys[MAX_KEYBOARD_KEYS]; // USB HID scan codes of currently pressed keys
};

class HIDKeyboard
{
public:
    HIDKeyboard(const std::shared_ptr<HIDReportDescriptor> &descriptor);
    ~HIDKeyboard();

    bool is_valid();
    uint8_t get_count();

    bool parse_data(uint8_t *data, uint16_t datalen, HIDKeyboardData *keyboard_data);

private:
    std::vector<HIDIOReport> m_reports;
};
