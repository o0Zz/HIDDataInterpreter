
#include <stdint.h>
#include <memory>
#include <vector>
#include <string>
#include "HIDReportDescriptor.h"

#define MAX_BUTTONS 32

#define JOYSTICK_SUPPORT_X 0x01
#define JOYSTICK_SUPPORT_Y 0x02

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

class HIDJoystickStick
{
public:
	HIDJoystickStick() :
		X(0),
		Y(0),
		pressed(false)
	{
	}
	~HIDJoystickStick() {}
	
	uint8_t X;
	uint8_t Y;
	bool pressed;
	uint8_t support;
};

class HIDJoystickData
{
public:
	HIDJoystickData();
	~HIDJoystickData();

	uint8_t index;

	uint8_t button_count;
	uint8_t buttons[MAX_BUTTONS];
	
	uint8_t stick_count;
	HIDJoystickStick sticks[8];

	bool has_hat_switch;
	HIDJoystickHatSwitch hat_switch;
};

class HIDJoystick
{
public:
	HIDJoystick(std::shared_ptr<HIDReportDescriptor> descriptor);
	~HIDJoystick();

	uint8_t getCount();

	bool parseData(uint8_t *data, uint16_t datalen, HIDJoystickData *joystick_data);

private:
	std::shared_ptr<HIDReportDescriptor> descriptor;
};