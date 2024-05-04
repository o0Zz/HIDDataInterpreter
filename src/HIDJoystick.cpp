#include "HIDJoystick.h"
#include "HIDUtils.h"

HIDJoystickData::HIDJoystickData() :
	index(0xFF),
	support(0),
	X(0),
	Y(0),
	Z(0),
	Rx(0),
	Ry(0),
	Rz(0),
	hat_switch(HIDJoystickHatSwitch::NEUTRAL),
	button_count(0)
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
		if (report->report_type == HIDIOReportType::Joystick
			|| report->report_type == HIDIOReportType::GamePad)
			count++;
	}

	return count;
}

/* ----------------------------------------------- */

bool HIDJoystick::parseData(uint8_t *data, uint16_t datalen, HIDJoystickData *joystick_data) 
{
	bool found = false;

	for (uint32_t i=0; i< descriptor->GetReports().size(); i++)
	{
		auto report = descriptor->GetReports()[i];

		if (report->report_type != HIDIOReportType::Joystick && report->report_type != HIDIOReportType::GamePad)
			continue;

		for (auto ioblock: report->inputs)
		{
			uint32_t bitOffset = 0;

			for (auto input: ioblock.data)
			{
				uint32_t value = HIDUtils::readBits(data, bitOffset, input.size);
				bitOffset += input.size;

				if (input.type == HIDIOType::ReportId)
				{
					if (value != input.id)
						break; //Not the correct report id
				}
				
				found = true;
				joystick_data->index = i;

				if (input.type == HIDIOType::Button)
				{
					if (input.id >= MAX_BUTTONS)
						return false;

					joystick_data->buttons[input.id] = value;
					if (joystick_data->button_count < input.id)
						joystick_data->button_count = input.id;
				}
				else if (input.type == HIDIOType::X)
				{
					joystick_data->support |= JOYSTICK_SUPPORT_X;
					joystick_data->X =value;
				}
				else if (input.type == HIDIOType::Y)
				{
					joystick_data->support |= JOYSTICK_SUPPORT_Y;
					joystick_data->Y = value;
				}
				else if (input.type == HIDIOType::Z)
				{
					joystick_data->support |= JOYSTICK_SUPPORT_Z;
					joystick_data->Z = value;
				}
				else if (input.type == HIDIOType::Rx)
				{
					joystick_data->support |= JOYSTICK_SUPPORT_Rx;
					joystick_data->Rx = value;
				}
				else if (input.type == HIDIOType::Ry)
				{
					joystick_data->support |= JOYSTICK_SUPPORT_Ry;
					joystick_data->Ry = value;
				}
				else if (input.type == HIDIOType::Rz)
				{
					joystick_data->support |= JOYSTICK_SUPPORT_Rz;
					joystick_data->Rz = value;
				}
				else if (input.type == HIDIOType::HatSwitch)
				{
					joystick_data->support |= JOYSTICK_SUPPORT_HatSwitch;
					joystick_data->hat_switch = (HIDJoystickHatSwitch)value;
				}
			}

			if (found)
				return true;
		}
	}

	return false;
}
