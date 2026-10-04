#include <gtest/gtest.h>
#include <stdint.h>
#include <stdbool.h>

extern "C" {
    bool bit_is_set(uint32_t reg, uint8_t bit);
}

TEST(CheckBitTest, StandardBits) {
    uint32_t reg = 0x00000004U; // Bit 2 set
    EXPECT_TRUE(bit_is_set(reg, 2));
    EXPECT_FALSE(bit_is_set(reg, 0));
    EXPECT_FALSE(bit_is_set(reg, 1));
    EXPECT_FALSE(bit_is_set(reg, 3));
}

TEST(CheckBitTest, BoundaryBits) {
    uint32_t reg = 0x80000001U; // Bit 0 (LSB) and Bit 31 (MSB) set
    EXPECT_TRUE(bit_is_set(reg, 0));
    EXPECT_TRUE(bit_is_set(reg, 31));
    EXPECT_FALSE(bit_is_set(reg, 1));
    EXPECT_FALSE(bit_is_set(reg, 30));
}

TEST(CheckBitTest, AllBitsSetAndZero) {
    uint32_t all_ones = 0xFFFFFFFFU;
    for (uint8_t i = 0; i < 32; ++i) {
        EXPECT_TRUE(bit_is_set(all_ones, i));
    }

    uint32_t all_zeros = 0x00000000U;
    for (uint8_t i = 0; i < 32; ++i) {
        EXPECT_FALSE(bit_is_set(all_zeros, i));
    }
}

TEST(CheckBitTest, OutOfBoundsBit) {
    uint32_t reg = 0xFFFFFFFFU;
    EXPECT_FALSE(bit_is_set(reg, 32));
    EXPECT_FALSE(bit_is_set(reg, 33));
    EXPECT_FALSE(bit_is_set(reg, 255));
}
