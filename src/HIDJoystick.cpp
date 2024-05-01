#include "HIDJoystick.h"
#include <HIDUtils.h>

HIDJoystick::HIDJoystick(std::shared_ptr<HIDReportDescriptor> descriptor) {
	this->descriptor = descriptor;
}

/* ----------------------------------------------- */

HIDJoystick::~HIDJoystick() {
}

/* ----------------------------------------------- */

bool HIDJoystick::parseData(uint8_t *data, uint16_t datalen, HIDJoystickData *joystick_data) 
{
	uint32_t bitOffset = 0;
	
	memset(joystick_data, 0, sizeof(HIDJoystickData));

	for (auto report: descriptor->GetReports())
	{
		for (auto input: report->inputs.data)
		{
			uint32_t value = HIDUtils::readBits(data, bitOffset, input.size);
			bitOffset += input.size;

			if (input.type == HIDIOType::ReportId)
			{
				if (value != input.id)
					break; //Not the correct report id
			}
			else if (input.type == HIDIOType::Button)
			{
				if (input.id >= MAX_BUTTONS)
					return false;

				joystick_data->buttons[input.id] = value;
				if (joystick_data->button_count < input.id)
					joystick_data->button_count = input.id;
			}
			else if (input.type == HIDIOType::X)
			{
				joystick_data->sticks[0].X = value;
			}
			else if (input.type == HIDIOType::Y)
			{
				joystick_data->sticks[0].Y = value;
			}
			else if (input.type == HIDIOType::Padding || input.type == HIDIOType::Unknown)
			{
				//Nothing to do
			}
		}
		

		return true;

	}
	return false;
}
