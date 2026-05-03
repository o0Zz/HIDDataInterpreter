#include "HIDReportDescriptor.h"
#include <memory>
#include <map>

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

#define JOYSTICK_SUPPORT_X            0x0001
#define JOYSTICK_SUPPORT_Y            0x0002
#define JOYSTICK_SUPPORT_Z            0x0004
#define JOYSTICK_SUPPORT_RX           0x0008
#define JOYSTICK_SUPPORT_RY           0x0010
#define JOYSTICK_SUPPORT_RZ           0x0020
#define JOYSTICK_SUPPORT_SLIDER       0x0040
#define JOYSTICK_SUPPORT_DIAL         0x0080
#define JOYSTICK_SUPPORT_BRAKE        0x0100
#define JOYSTICK_SUPPORT_ACCELERATOR  0x0200
#define JOYSTICK_SUPPORT_HAT_SWITCH   0x0400

class HIDJoystickData
{
public:
    HIDJoystickData();
    ~HIDJoystickData();

    uint8_t index;

    uint16_t support;

    int16_t x;  //-32768 to 32767
    int16_t y;  //-32768 to 32767
    int16_t z;  //-32768 to 32767
    int16_t rx; //-32768 to 32767
    int16_t ry; //-32768 to 32767
    int16_t rz; //-32768 to 32767
    int16_t slider; //-32768 to 32767
    int16_t dial; //-32768 to 32767
    int16_t brake; //-32768 to 32767
    int16_t accelerator; //-32768 to 32767

    HIDJoystickHatSwitch hat_switch;

    uint8_t button_count;
    uint8_t buttons[MAX_BUTTONS];

    std::map<HIDIOConsumerType, uint8_t> consumer_buttons;
};

class HIDJoystick
{
public:
    HIDJoystick(const std::shared_ptr<HIDReportDescriptor> &descriptor);
    ~HIDJoystick();

    bool is_valid();
    uint8_t get_count();

    bool parse_data(uint8_t *data, uint16_t datalen, HIDJoystickData *joystick_data);

private:
    std::vector<HIDIOReport> m_reports;
};