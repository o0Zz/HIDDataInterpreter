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
		if (report->report_id == 0 || report->report_id == data[0])
		{
			if (report->report_id != 0)
				bitOffset = 8;

			for (auto input: report->inputs)
			{
				for (uint32_t i = 0; i < input.count; i++)
				{
					uint32_t value = HIDUtils::readBits(data, bitOffset, input.size);
					bitOffset += input.size;

					if (input.type == HIDInputType::Button)
					{
						joystick_data->buttons[i] = value;
						joystick_data->button_count++;
					}
					else if (input.type == HIDInputType::X)
					{
						joystick_data->sticks[i].X = value;
					}
					else if (input.type == HIDInputType::Y)
					{
						joystick_data->sticks[i].Y = value;
					}
					else if (input.type == HIDInputType::Padding)
					{
						//Nothing to do
					}
				}
			}

			return true;
		}
	}
	return false;
}
