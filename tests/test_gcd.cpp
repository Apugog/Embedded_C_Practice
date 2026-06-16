#include <gtest/gtest.h>
#include <stdint.h>

extern "C" {
    uint32_t findGCD(int32_t a, int32_t b);
}

TEST(GCDTests, HandlesNormalInputs) {
    EXPECT_EQ(findGCD(24, 36), 12);
    EXPECT_EQ(findGCD(36, 24), 12);
}

TEST(GCDTests, HandlesPrimeInputs) {
    EXPECT_EQ(findGCD(17, 23), 1);
}

TEST(GCDTests, HandlesOneNegativeInput) {
    EXPECT_EQ(findGCD(-24, 36), 12);
    EXPECT_EQ(findGCD(24, -36), 12);
}

TEST(GCDTests, HandlesBothNegativeInputs) {
    EXPECT_EQ(findGCD(-24, -36), 12);
}

TEST(GCDTests, HandlesZero) {
    EXPECT_EQ(findGCD(0, 5), 5);
    EXPECT_EQ(findGCD(5, 0), 5);
    EXPECT_EQ(findGCD(0, 0), 0);
}

TEST(GCDTests, HandlesInt32Max) {
    EXPECT_EQ(findGCD(INT32_MAX, 1), 1);
    EXPECT_EQ(findGCD(INT32_MAX, INT32_MAX), (uint32_t)INT32_MAX);
}

TEST(GCDTests, HandlesInt32Min) {
    // Math GCD of INT32_MIN (-2147483648) and 0 is 2147483648
    EXPECT_EQ(findGCD(INT32_MIN, 0), 2147483648U);
    EXPECT_EQ(findGCD(INT32_MIN, INT32_MIN), 2147483648U);
    EXPECT_EQ(findGCD(INT32_MIN, 4), 4);
    EXPECT_EQ(findGCD(INT32_MIN, -4), 4);
}
