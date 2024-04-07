/*Joystick(HidReportDescriptor descriptor)
support(PadX)

::parseData(JoystickData *data)


JoystickStick
{
	uint8_t X;
	uint8_t Y;
	bool pressed;
}

JoystickData
{
	uint8_t index;
	
	uint8_t button_count;
	uint8_t buttons[32];
	
	uint8_t stick_count;
	JoystickStick sticks[8];
}


dataOffset
{
	uint8_t OffsetInBit;
	uint8_t SizeInBit;
}


uint8_t DataParser(bit, size)
{
	
}



getJoystickCount()

getButtonCount(uint8_t input_idx)


getPadX()
getPadX()





Button1Offset()
*/