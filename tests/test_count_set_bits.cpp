#include <gtest/gtest.h>
#include <stdint.h>

extern "C" {
    uint8_t count_bits(uint32_t value);
    uint8_t count_bits_swar(uint32_t value);
}

// -----------------------------------------------------------------------------
// Test Suite: Brian Kernighan Algorithm (count_bits)
// -----------------------------------------------------------------------------

TEST(CountBitsTest, ZeroValue) {
    EXPECT_EQ(count_bits(0x00000000U), 0);
}

TEST(CountBitsTest, AllBitsSet) {
    EXPECT_EQ(count_bits(0xFFFFFFFFU), 32);
}

TEST(CountBitsTest, SingleBitPowersOfTwo) {
    for (uint8_t i = 0; i < 32; ++i) {
        EXPECT_EQ(count_bits(1U << i), 1);
    }
}

TEST(CountBitsTest, InvertedPowersOfTwo) {
    for (uint8_t i = 0; i < 32; ++i) {
        EXPECT_EQ(count_bits(~(1U << i)), 31);
    }
}

TEST(CountBitsTest, BoundaryBits) {
    EXPECT_EQ(count_bits(0x80000001U), 2); // LSB (bit 0) and MSB (bit 31)
    EXPECT_EQ(count_bits(0xC0000003U), 4); // Top 2 bits and Bottom 2 bits
    EXPECT_EQ(count_bits(0xF000000FU), 8); // Top 4 bits and Bottom 4 bits
}

TEST(CountBitsTest, AlternatingBits) {
    EXPECT_EQ(count_bits(0xAAAAAAAAU), 16);
    EXPECT_EQ(count_bits(0x55555555U), 16);
    EXPECT_EQ(count_bits(0xCCCCCCCCU), 16);
    EXPECT_EQ(count_bits(0x33333333U), 16);
}

TEST(CountBitsTest, ArbitraryPatterns) {
    EXPECT_EQ(count_bits(0x000000B1U), 4); // 0b10110001
    EXPECT_EQ(count_bits(0x12345678U), 13);
    EXPECT_EQ(count_bits(0xDEADBEEFU), 24);
    EXPECT_EQ(count_bits(0xCAFEBABEU), 22);
    EXPECT_EQ(count_bits(0x0F0F0F0FU), 16);
    EXPECT_EQ(count_bits(0xF0F0F0F0U), 16);
}

// -----------------------------------------------------------------------------
// Test Suite: SWAR Parallel Popcount (count_bits_swar)
// -----------------------------------------------------------------------------

TEST(CountBitsSwarTest, ZeroValue) {
    EXPECT_EQ(count_bits_swar(0x00000000U), 0);
}

TEST(CountBitsSwarTest, AllBitsSet) {
    EXPECT_EQ(count_bits_swar(0xFFFFFFFFU), 32);
}

TEST(CountBitsSwarTest, SingleBitPowersOfTwo) {
    for (uint8_t i = 0; i < 32; ++i) {
        EXPECT_EQ(count_bits_swar(1U << i), 1);
    }
}

TEST(CountBitsSwarTest, InvertedPowersOfTwo) {
    for (uint8_t i = 0; i < 32; ++i) {
        EXPECT_EQ(count_bits_swar(~(1U << i)), 31);
    }
}

TEST(CountBitsSwarTest, BoundaryBits) {
    EXPECT_EQ(count_bits_swar(0x80000001U), 2);
    EXPECT_EQ(count_bits_swar(0xC0000003U), 4);
    EXPECT_EQ(count_bits_swar(0xF000000FU), 8);
}

TEST(CountBitsSwarTest, AlternatingBits) {
    EXPECT_EQ(count_bits_swar(0xAAAAAAAAU), 16);
    EXPECT_EQ(count_bits_swar(0x55555555U), 16);
    EXPECT_EQ(count_bits_swar(0xCCCCCCCCU), 16);
    EXPECT_EQ(count_bits_swar(0x33333333U), 16);
}

TEST(CountBitsSwarTest, BytePatterns) {
    EXPECT_EQ(count_bits_swar(0x000000FFU), 8);
    EXPECT_EQ(count_bits_swar(0x0000FFFFU), 16);
    EXPECT_EQ(count_bits_swar(0x00FFFFFFU), 24);
    EXPECT_EQ(count_bits_swar(0x0F0F0F0FU), 16);
    EXPECT_EQ(count_bits_swar(0xF0F0F0F0U), 16);
}

TEST(CountBitsSwarTest, ArbitraryPatterns) {
    EXPECT_EQ(count_bits_swar(0x000000B1U), 4); // 0b10110001
    EXPECT_EQ(count_bits_swar(0x12345678U), 13);
    EXPECT_EQ(count_bits_swar(0xDEADBEEFU), 24);
    EXPECT_EQ(count_bits_swar(0xCAFEBABEU), 22);
}

// -----------------------------------------------------------------------------
// Test Suite: Differential Equivalence (SWAR vs Kernighan)
// -----------------------------------------------------------------------------

TEST(CountBitsDifferentialTest, CumulativeBitFills) {
    uint32_t fill = 0;
    for (uint8_t i = 0; i < 32; ++i) {
        fill = (fill << 1U) | 1U;
        EXPECT_EQ(count_bits_swar(fill), count_bits(fill));
        EXPECT_EQ(count_bits_swar(fill), i + 1);
    }
}

TEST(CountBitsDifferentialTest, SlidingWindowsAndCombinations) {
    // Sliding 8-bit mask across all 32 positions
    for (uint8_t shift = 0; shift <= 24; ++shift) {
        uint32_t val = 0xFFU << shift;
        EXPECT_EQ(count_bits_swar(val), count_bits(val));
        EXPECT_EQ(count_bits_swar(val), 8);
    }

    // Walking bit-pairs
    for (uint8_t i = 0; i < 31; ++i) {
        uint32_t val = 3U << i;
        EXPECT_EQ(count_bits_swar(val), count_bits(val));
        EXPECT_EQ(count_bits_swar(val), 2);
    }
}
