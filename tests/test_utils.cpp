#include <gtest/gtest.h>

class HIDUtils
{
public:
	static uint32_t readBitsLE(uint8_t *buffer, uint32_t bitOffset, uint32_t bitLength);
};

TEST(UTILS, test_read_bits)
{
	uint32_t bitOffset = 0;
	uint32_t bitLength = 0;
	uint32_t value = 0;

	// Buffer n1 tests
	uint8_t buffer[2] = {0x01, 0x03};

	value = HIDUtils::readBitsLE(buffer, 0/*bitOffset*/, 9 /*bitLength*/);
	EXPECT_EQ(value, 0x101);

	value = HIDUtils::readBitsLE(buffer, 0/*bitOffset*/, 8 /*bitLength*/);
	EXPECT_EQ(value, 0x01);

	value = HIDUtils::readBitsLE(buffer, 1/*bitOffset*/, 7 /*bitLength*/);
	EXPECT_EQ(value, 0x00);

	value = HIDUtils::readBitsLE(buffer, 0/*bitOffset*/, 16 /*bitLength*/);
	EXPECT_EQ(value, 0x0301);

	value = HIDUtils::readBitsLE(buffer, 1/*bitOffset*/, 8 /*bitLength*/);
	EXPECT_EQ(value, 0x80);
	
	//Buffer n2 tests
	uint8_t buffer2[2] = {0x54, 0x01};
	value = HIDUtils::readBitsLE(buffer2, 0/*bitOffset*/, 10 /*bitLength*/);
	EXPECT_EQ(value, 0x154);

}