#include "HIDJoystick.h"
#include "HIDUtils.h"

HIDJoystickData::HIDJoystickData() :
	index(0),
	button_count(0),
	stick_count(0),
	has_hat_switch(false),
	hat_switch(HIDJoystickHatSwitch::NEUTRAL)
{
	memset(buttons, 0, sizeof(buttons));
}

/* ----------------------------------------------- */

HIDJoystickData::~HIDJoystickData() 
{

}

/* ----------------------------------------------- */


HIDJoystick::HIDJoystick(std::shared_ptr<HIDReportDescriptor> descriptor) 
{
	this->descriptor = descriptor;
}

/* ----------------------------------------------- */

HIDJoystick::~HIDJoystick() 
{
}

/* ----------------------------------------------- */

uint8_t HIDJoystick::getCount()
{
	uint8_t count = 0;

	for (auto report: descriptor->GetReports())
	{
		if (report->report_type == HIDReportType::Joystick
			|| report->report_type == HIDReportType::GamePad)
			count++;
	}

	return count;
}

/* ----------------------------------------------- */

bool HIDJoystick::parseData(uint8_t *data, uint16_t datalen, HIDJoystickData *joystick_data) 
{
	uint32_t bitOffset = 0;

	memset(joystick_data, 0, sizeof(HIDJoystickData));

	for (auto report: descriptor->GetReports())
	{
		if (report->report_type != HIDReportType::Joystick && report->report_type != HIDReportType::GamePad)
			continue;

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
				if (joystick_data->sticks[joystick_data->stick_count].support & JOYSTICK_SUPPORT_X)
					joystick_data->stick_count++; //If we already set X it means it's a new stick

				joystick_data->sticks[joystick_data->stick_count].support |= JOYSTICK_SUPPORT_X;
				joystick_data->sticks[joystick_data->stick_count].X = value;
			}
			else if (input.type == HIDIOType::Y)
			{
				if (joystick_data->sticks[joystick_data->stick_count].support & JOYSTICK_SUPPORT_Y)
					joystick_data->stick_count++; //If we already set Y it means it's a new stick

				joystick_data->sticks[joystick_data->stick_count].support |= JOYSTICK_SUPPORT_Y;
				joystick_data->sticks[joystick_data->stick_count].Y = value;
			}
			else if (input.type == HIDIOType::HatSwitch)
			{
				joystick_data->has_hat_switch = true;
				joystick_data->hat_switch = (HIDJoystickHatSwitch)value;
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
