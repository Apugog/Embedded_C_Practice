#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {
    typedef enum {
        BIT_RNG_OK,
        BIT_RNG_NULL,
        BIT_RNG_INVALID_INPUT
    } bit_rng_status_t;

    bit_rng_status_t set_bit_range(uint32_t *value, size_t start, size_t end);
    bit_rng_status_t clear_bit_range(uint32_t *value, size_t start, size_t end);
}

/* --------------------------------------------------------------------------
 * 1. set_bit_range Tests
 * -------------------------------------------------------------------------- */
TEST(BitRangeTest, SetBitRange_StandardRanges) {
    uint32_t val = 0x00000000U;

    // Bits [8:7] -> expected bits 8 and 7 set (0x100 | 0x80 = 0x180)
    EXPECT_EQ(set_bit_range(&val, 8, 7), BIT_RNG_OK);
    EXPECT_EQ(val, 0x00000180U);

    // Byte 1: Bits [15:8]
    val = 0x00000000U;
    EXPECT_EQ(set_bit_range(&val, 15, 8), BIT_RNG_OK);
    EXPECT_EQ(val, 0x0000FF00U);

    // Nibble 0: Bits [3:0]
    val = 0x00000000U;
    EXPECT_EQ(set_bit_range(&val, 3, 0), BIT_RNG_OK);
    EXPECT_EQ(val, 0x0000000FU);
}

TEST(BitRangeTest, SetBitRange_SingleBitBoundaries) {
    uint32_t val = 0x00000000U;

    // LSB: Bit 0 [0:0]
    EXPECT_EQ(set_bit_range(&val, 0, 0), BIT_RNG_OK);
    EXPECT_EQ(val, 0x00000001U);

    // Mid: Bit 16 [16:16]
    val = 0x00000000U;
    EXPECT_EQ(set_bit_range(&val, 16, 16), BIT_RNG_OK);
    EXPECT_EQ(val, 0x00010000U);

    // MSB: Bit 31 [31:31]
    val = 0x00000000U;
    EXPECT_EQ(set_bit_range(&val, 31, 31), BIT_RNG_OK);
    EXPECT_EQ(val, 0x80000000U);
}

TEST(BitRangeTest, SetBitRange_FullRegister) {
    uint32_t val = 0x00000000U;

    // All 32 bits [31:0]
    EXPECT_EQ(set_bit_range(&val, 31, 0), BIT_RNG_OK);
    EXPECT_EQ(val, 0xFFFFFFFFU);
}

TEST(BitRangeTest, SetBitRange_IdempotenceAndAccumulation) {
    uint32_t val = 0x000000F0U; // Bits [7:4] already set

    // Setting overlapping bits [7:0]
    EXPECT_EQ(set_bit_range(&val, 7, 0), BIT_RNG_OK);
    EXPECT_EQ(val, 0x000000FFU);

    // Setting already set range [7:0] should be idempotent
    EXPECT_EQ(set_bit_range(&val, 7, 0), BIT_RNG_OK);
    EXPECT_EQ(val, 0x000000FFU);

    // Setting non-overlapping high nibble [31:28]
    EXPECT_EQ(set_bit_range(&val, 31, 28), BIT_RNG_OK);
    EXPECT_EQ(val, 0xF00000FFU);
}

/* --------------------------------------------------------------------------
 * 2. clear_bit_range Tests
 * -------------------------------------------------------------------------- */
TEST(BitRangeTest, ClearBitRange_StandardRanges) {
    uint32_t val = 0xFFFFFFFFU;

    // Clear bits [8:7] -> mask is 0x180, inverted is 0xFFFFFE7F
    EXPECT_EQ(clear_bit_range(&val, 8, 7), BIT_RNG_OK);
    EXPECT_EQ(val, 0xFFFFFE7FU);

    // Clear byte 1: Bits [15:8]
    val = 0xFFFFFFFFU;
    EXPECT_EQ(clear_bit_range(&val, 15, 8), BIT_RNG_OK);
    EXPECT_EQ(val, 0xFFFF00FFU);

    // Clear low nibble: Bits [3:0]
    val = 0xFFFFFFFFU;
    EXPECT_EQ(clear_bit_range(&val, 3, 0), BIT_RNG_OK);
    EXPECT_EQ(val, 0xFFFFFFF0U);
}

TEST(BitRangeTest, ClearBitRange_SingleBitBoundaries) {
    uint32_t val = 0xFFFFFFFFU;

    // LSB: Bit 0 [0:0]
    EXPECT_EQ(clear_bit_range(&val, 0, 0), BIT_RNG_OK);
    EXPECT_EQ(val, 0xFFFFFFFEU);

    // Mid: Bit 16 [16:16]
    val = 0xFFFFFFFFU;
    EXPECT_EQ(clear_bit_range(&val, 16, 16), BIT_RNG_OK);
    EXPECT_EQ(val, 0xFFFEFFFFU);

    // MSB: Bit 31 [31:31]
    val = 0xFFFFFFFFU;
    EXPECT_EQ(clear_bit_range(&val, 31, 31), BIT_RNG_OK);
    EXPECT_EQ(val, 0x7FFFFFFFU);
}

TEST(BitRangeTest, ClearBitRange_FullRegister) {
    uint32_t val = 0xFFFFFFFFU;

    // Clear all 32 bits [31:0]
    EXPECT_EQ(clear_bit_range(&val, 31, 0), BIT_RNG_OK);
    EXPECT_EQ(val, 0x00000000U);
}

TEST(BitRangeTest, ClearBitRange_Idempotence) {
    uint32_t val = 0x00000000U;

    // Clearing bits on an already cleared register
    EXPECT_EQ(clear_bit_range(&val, 15, 8), BIT_RNG_OK);
    EXPECT_EQ(val, 0x00000000U);
}

/* --------------------------------------------------------------------------
 * 3. Error Handling & Parameter Validation
 * -------------------------------------------------------------------------- */
TEST(BitRangeTest, NullPointerGuards) {
    EXPECT_EQ(set_bit_range(NULL, 8, 7), BIT_RNG_NULL);
    EXPECT_EQ(clear_bit_range(NULL, 8, 7), BIT_RNG_NULL);
    EXPECT_EQ(set_bit_range(NULL, 0, 0), BIT_RNG_NULL);
    EXPECT_EQ(clear_bit_range(NULL, 31, 31), BIT_RNG_NULL);
}

TEST(BitRangeTest, OutOfBoundsAndInvertedRanges) {
    uint32_t val = 0x12345678U;

    // start > 31
    EXPECT_EQ(set_bit_range(&val, 32, 0), BIT_RNG_INVALID_INPUT);
    EXPECT_EQ(clear_bit_range(&val, 32, 0), BIT_RNG_INVALID_INPUT);
    EXPECT_EQ(val, 0x12345678U); // Verify value unmodified

    // end > 31
    EXPECT_EQ(set_bit_range(&val, 30, 32), BIT_RNG_INVALID_INPUT);
    EXPECT_EQ(clear_bit_range(&val, 30, 32), BIT_RNG_INVALID_INPUT);
    EXPECT_EQ(val, 0x12345678U);

    // Inverted range: end > start (e.g. start=0, end=7)
    EXPECT_EQ(set_bit_range(&val, 0, 7), BIT_RNG_INVALID_INPUT);
    EXPECT_EQ(clear_bit_range(&val, 0, 7), BIT_RNG_INVALID_INPUT);
    EXPECT_EQ(val, 0x12345678U);

    // Large out of bounds
    EXPECT_EQ(set_bit_range(&val, 100, 50), BIT_RNG_INVALID_INPUT);
    EXPECT_EQ(clear_bit_range(&val, 100, 50), BIT_RNG_INVALID_INPUT);
    EXPECT_EQ(val, 0x12345678U);
}

/* --------------------------------------------------------------------------
 * 4. Exhaustive Differential Verification Across All Valid Subranges
 * -------------------------------------------------------------------------- */
TEST(BitRangeTest, DifferentialFuzzAllValidRanges) {
    for (size_t start = 0; start < 32; ++start) {
        for (size_t end = 0; end <= start; ++end) {
            uint32_t reg = 0x00000000U;
            EXPECT_EQ(set_bit_range(&reg, start, end), BIT_RNG_OK);

            // Compute expected bitmask by setting each bit individually
            uint32_t expected = 0;
            for (size_t b = end; b <= start; ++b) {
                expected |= (1U << b);
            }
            EXPECT_EQ(reg, expected) << "Mismatch at start=" << start << ", end=" << end;

            // Clear the same range and verify register returns to 0
            EXPECT_EQ(clear_bit_range(&reg, start, end), BIT_RNG_OK);
            EXPECT_EQ(reg, 0x00000000U) << "Clear mismatch at start=" << start << ", end=" << end;
        }
    }
}
