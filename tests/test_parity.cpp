#include <gtest/gtest.h>
#include <stdint.h>

typedef enum {
    PARITY_EVEN = 0,
    PARITY_ODD  = 1
} parity_t;

extern "C" {
    parity_t check_parity(uint32_t val);
}

// -----------------------------------------------------------------------------
// Reference Naive Parity for Differential Verification
// -----------------------------------------------------------------------------
static parity_t reference_parity(uint32_t val) {
    uint8_t count = 0;
    while (val != 0U) {
        val &= (val - 1U);
        count++;
    }
    return (count % 2U == 0U) ? PARITY_EVEN : PARITY_ODD;
}

// -----------------------------------------------------------------------------
// Test Suite: Basic & Extremes
// -----------------------------------------------------------------------------
TEST(ParityTest, ZeroAndAllBitsSet) {
    EXPECT_EQ(check_parity(0x00000000U), PARITY_EVEN); // 0 set bits (even)
    EXPECT_EQ(check_parity(0xFFFFFFFFU), PARITY_EVEN); // 32 set bits (even)
}

// -----------------------------------------------------------------------------
// Test Suite: Powers of Two (Single Bit Set -> Odd Parity)
// -----------------------------------------------------------------------------
TEST(ParityTest, SingleBitPowersOfTwo) {
    for (uint8_t i = 0; i < 32; ++i) {
        uint32_t val = 1U << i;
        EXPECT_EQ(check_parity(val), PARITY_ODD) << "Failed for bit position: " << (int)i;
    }
}

// -----------------------------------------------------------------------------
// Test Suite: Two Bits Set -> Even Parity
// -----------------------------------------------------------------------------
TEST(ParityTest, TwoBitsSet) {
    for (uint8_t i = 0; i < 31; ++i) {
        uint32_t val = (1U << i) | (1U << (i + 1));
        EXPECT_EQ(check_parity(val), PARITY_EVEN) << "Failed for adjacent bits: " << (int)i;
    }
}

// -----------------------------------------------------------------------------
// Test Suite: Inverted Powers of Two (31 Bits Set -> Odd Parity)
// -----------------------------------------------------------------------------
TEST(ParityTest, InvertedPowersOfTwo) {
    for (uint8_t i = 0; i < 32; ++i) {
        uint32_t val = ~(1U << i);
        EXPECT_EQ(check_parity(val), PARITY_ODD) << "Failed for inverted bit: " << (int)i;
    }
}

// -----------------------------------------------------------------------------
// Test Suite: Boundary Bits
// -----------------------------------------------------------------------------
TEST(ParityTest, BoundaryBits) {
    EXPECT_EQ(check_parity(0x80000000U), PARITY_ODD);  // MSB only (1 bit)
    EXPECT_EQ(check_parity(0x00000001U), PARITY_ODD);  // LSB only (1 bit)
    EXPECT_EQ(check_parity(0x80000001U), PARITY_EVEN); // MSB + LSB (2 bits)
    EXPECT_EQ(check_parity(0xC0000001U), PARITY_ODD);  // Bits 31, 30, 0 (3 bits)
    EXPECT_EQ(check_parity(0xC0000003U), PARITY_EVEN); // Bits 31, 30, 1, 0 (4 bits)
    EXPECT_EQ(check_parity(0xF000000FU), PARITY_EVEN); // 8 bits
    EXPECT_EQ(check_parity(0x7000000FU), PARITY_ODD);  // 7 bits
}

// -----------------------------------------------------------------------------
// Test Suite: Alternating Patterns
// -----------------------------------------------------------------------------
TEST(ParityTest, AlternatingPatterns) {
    EXPECT_EQ(check_parity(0xAAAAAAAAU), PARITY_EVEN); // 16 bits
    EXPECT_EQ(check_parity(0x55555555U), PARITY_EVEN); // 16 bits
    EXPECT_EQ(check_parity(0xCCCCCCCCU), PARITY_EVEN); // 16 bits
    EXPECT_EQ(check_parity(0x33333333U), PARITY_EVEN); // 16 bits
    EXPECT_EQ(check_parity(0x0F0F0F0FU), PARITY_EVEN); // 16 bits
    EXPECT_EQ(check_parity(0xF0F0F0F0U), PARITY_EVEN); // 16 bits

    // Toggle one bit in each alternating pattern -> must become ODD
    EXPECT_EQ(check_parity(0xAAAAAAA8U), PARITY_ODD);  // 15 bits
    EXPECT_EQ(check_parity(0x55555554U), PARITY_ODD);  // 15 bits
}

// -----------------------------------------------------------------------------
// Test Suite: Arbitrary Patterns
// -----------------------------------------------------------------------------
TEST(ParityTest, ArbitraryPatterns) {
    EXPECT_EQ(check_parity(0x000000B1U), PARITY_EVEN); // 0b10110001 (4 set bits)
    EXPECT_EQ(check_parity(0x12345678U), PARITY_ODD);  // 13 set bits
    EXPECT_EQ(check_parity(0xDEADBEEFU), PARITY_EVEN); // 24 set bits
    EXPECT_EQ(check_parity(0xCAFEBABEU), PARITY_EVEN); // 22 set bits
}

// -----------------------------------------------------------------------------
// Test Suite: Differential Fuzz (Exhaustive 16-bit range + shifts)
// -----------------------------------------------------------------------------
TEST(ParityTest, DifferentialFuzzExhaustive16Bit) {
    for (uint32_t val = 0; val <= 0xFFFFU; ++val) {
        ASSERT_EQ(check_parity(val), reference_parity(val));
    }
}

TEST(ParityTest, DifferentialFuzzShiftedRanges) {
    for (uint32_t val = 0; val <= 0x0FFFU; ++val) {
        uint32_t shifted = val << 16U;
        ASSERT_EQ(check_parity(shifted), reference_parity(shifted));

        uint32_t combined = (val << 16U) | (val ^ 0x0AA5U);
        ASSERT_EQ(check_parity(combined), reference_parity(combined));
    }
}
