
#include <stdint.h>
#include <memory>
#include <vector>
#include <string>
#include "HIDReportDescriptor.h"

#define MAX_BUTTONS 32
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
};

class HIDJoystickData
{
public:
	HIDJoystickData()
	{
	}
	~HIDJoystickData() {}

	uint8_t index;

	uint8_t button_count;
	uint8_t buttons[MAX_BUTTONS];
	
	uint8_t stick_count;
	HIDJoystickStick sticks[8];
};

class HIDJoystick
{
	public:
		HIDJoystick(std::shared_ptr<HIDReportDescriptor> descriptor);
		~HIDJoystick();
		
		bool parseData(uint8_t *data, uint16_t datalen, HIDJoystickData *joystick_data);
		
		uint8_t getJoystickCount();
		
	private:
		std::shared_ptr<HIDReportDescriptor> descriptor;
};