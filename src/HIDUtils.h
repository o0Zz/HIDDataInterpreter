#pragma once
#include <stdint.h>

class HIDUtils
{
public:
	HIDUtils() {}
	~HIDUtils() {}

	static uint32_t read_bits_le(uint8_t *buffer, uint32_t bit_offset, uint32_t bit_length);
	static int32_t sign_extend(uint32_t value, uint32_t bits);
};