#include <gtest/gtest.h>

class HIDUtils
{
public:
	static uint32_t read_bits_le(uint8_t *buffer, uint32_t bit_offset, uint32_t bit_length);
};

TEST(UTILS, test_read_bits)
{
	uint32_t bit_offset = 0;
	uint32_t bit_length = 0;
	uint32_t value = 0;

	// Buffer n1 tests
	uint8_t buffer[2] = {0x01, 0x03};

	value = HIDUtils::read_bits_le(buffer, 0/*bit_offset*/, 9 /*bit_length*/);
	EXPECT_EQ(value, 0x101);

	value = HIDUtils::read_bits_le(buffer, 0/*bit_offset*/, 8 /*bit_length*/);
	EXPECT_EQ(value, 0x01);

	value = HIDUtils::read_bits_le(buffer, 1/*bit_offset*/, 7 /*bit_length*/);
	EXPECT_EQ(value, 0x00);

	value = HIDUtils::read_bits_le(buffer, 0/*bit_offset*/, 16 /*bit_length*/);
	EXPECT_EQ(value, 0x0301);

	value = HIDUtils::read_bits_le(buffer, 1/*bit_offset*/, 8 /*bit_length*/);
	EXPECT_EQ(value, 0x80);
	
	//Buffer n2 tests
	uint8_t buffer2[2] = {0x54, 0x01};
	value = HIDUtils::read_bits_le(buffer2, 0/*bit_offset*/, 10 /*bit_length*/);
	EXPECT_EQ(value, 0x154);

}