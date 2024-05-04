
#include <stdint.h>
#include <memory>
#include <vector>
#include <string>
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

#define JOYSTICK_SUPPORT_X 			0x01
#define JOYSTICK_SUPPORT_Y 			0x02
#define JOYSTICK_SUPPORT_Z 			0x04
#define JOYSTICK_SUPPORT_Rx 		0x08
#define JOYSTICK_SUPPORT_Ry 		0x10
#define JOYSTICK_SUPPORT_Rz 		0x20
#define JOYSTICK_SUPPORT_HatSwitch 	0x40

class HIDJoystickData
{
public:
	HIDJoystickData();
	~HIDJoystickData();

	uint8_t index;

	uint8_t support;

	uint8_t X;
	uint8_t Y;
	uint8_t Z;
	uint8_t Rx;
	uint8_t Ry;
	uint8_t Rz;
	HIDJoystickHatSwitch hat_switch;

	uint8_t button_count;
	uint8_t buttons[MAX_BUTTONS];
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