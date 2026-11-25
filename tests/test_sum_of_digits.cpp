#include <gtest/gtest.h>
#include <stdint.h>

extern "C" {
    uint32_t sum_of_digit(uint32_t num);
}

// Suite 1: Base Cases (0 and single-digit numbers)
TEST(SumOfDigitsTest, BaseCasesAndSingleDigits) {
    EXPECT_EQ(sum_of_digit(0U), 0U);
    EXPECT_EQ(sum_of_digit(1U), 1U);
    EXPECT_EQ(sum_of_digit(5U), 5U);
    EXPECT_EQ(sum_of_digit(9U), 9U);
}

// Suite 2: Multi-Digit Numbers
TEST(SumOfDigitsTest, MultiDigitNumbers) {
    EXPECT_EQ(sum_of_digit(12U), 3U);
    EXPECT_EQ(sum_of_digit(234U), 9U);
    EXPECT_EQ(sum_of_digit(999U), 27U);
    EXPECT_EQ(sum_of_digit(12345U), 15U);
    EXPECT_EQ(sum_of_digit(987654321U), 45U);
}

// Suite 3: Numbers Containing Zeros & Powers of 10
TEST(SumOfDigitsTest, ZerosAndPowersOfTen) {
    EXPECT_EQ(sum_of_digit(10U), 1U);
    EXPECT_EQ(sum_of_digit(100U), 1U);
    EXPECT_EQ(sum_of_digit(1000000000U), 1U);
    EXPECT_EQ(sum_of_digit(10203040U), 10U);
    EXPECT_EQ(sum_of_digit(900000009U), 18U);
}

// Suite 4: 32-bit Unsigned Boundary Values
TEST(SumOfDigitsTest, MaxUint32Boundaries) {
    EXPECT_EQ(sum_of_digit(999999999U), 81U);
    EXPECT_EQ(sum_of_digit(3999999999U), 84U);
    EXPECT_EQ(sum_of_digit(UINT32_MAX), 57U); // 4294967295 -> 4+2+9+4+9+6+7+2+9+5 = 57
}
