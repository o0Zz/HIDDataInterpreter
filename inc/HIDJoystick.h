#include "HIDReportDescriptor.h"

#define MAX_BUTTONS 32

enum class HIDJoystickHatSwitch
{
    UP = 0,
    UP_RIGHT = 1,
    RIGHT = 2,
    DOWN_RIGHT = 3,
    DOWN = 4,
    DOWN_LEFT = 5,
    LEFT = 6,
    UP_LEFT = 7,
    NEUTRAL = 8
};

#define JOYSTICK_SUPPORT_X         0x01
#define JOYSTICK_SUPPORT_Y         0x02
#define JOYSTICK_SUPPORT_Z         0x04
#define JOYSTICK_SUPPORT_Rx        0x08
#define JOYSTICK_SUPPORT_Ry        0x10
#define JOYSTICK_SUPPORT_Rz        0x20
#define JOYSTICK_SUPPORT_HatSwitch 0x40

class HIDJoystickData
{
public:
    HIDJoystickData();
    ~HIDJoystickData();

    uint8_t index;

    uint8_t support;

    int16_t X;
    int16_t Y;
    int16_t Z;
    int16_t Rx;
    int16_t Ry;
    int16_t Rz;
    HIDJoystickHatSwitch hat_switch;

    uint8_t button_count;
    uint8_t buttons[MAX_BUTTONS];
};

class HIDJoystick
{
public:
    HIDJoystick(const HIDReportDescriptor &descriptor);
    ~HIDJoystick();

    bool isValid();
    uint8_t getCount();

    bool parseData(uint8_t *data, uint16_t datalen, HIDJoystickData *joystick_data);

private:
    HIDReportDescriptor m_descriptor;
    std::vector<HIDIOReport> m_reports;
};