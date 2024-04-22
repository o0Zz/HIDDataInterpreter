#include <gtest/gtest.h>
#include "HIDUtils.h"

TEST(UTILS, test_read_bits)
{
	uint8_t buffer[2] = {0x01, 0x02};

	uint32_t bitOffset = 0;
	uint32_t bitLength = 8;
	uint32_t value = HIDUtils::readBits(buffer, bitOffset, bitLength);
	EXPECT_EQ(value, 0x01);

	bitOffset = 1;
	bitLength = 7;
	value = HIDUtils::readBits(buffer, bitOffset, bitLength);
	EXPECT_EQ(value, 0x01);

	bitOffset = 0;
	bitLength = 9;
	value = HIDUtils::readBits(buffer, bitOffset, bitLength);
	EXPECT_EQ(value, 0x02);

	bitOffset = 0;
	bitLength = 16;
	value = HIDUtils::readBits(buffer, bitOffset, bitLength);
	EXPECT_EQ(value, 0x0102);

	bitOffset = 1;
	bitLength = 8;
	value = HIDUtils::readBits(buffer, bitOffset, bitLength);
	EXPECT_EQ(value, 0x02);

}