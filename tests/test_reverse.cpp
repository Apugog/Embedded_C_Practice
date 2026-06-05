#include <gtest/gtest.h>
#include <stdint.h>

// Link the C implementation
extern "C" {
    int8_t reverseNum(int32_t num, int32_t *result);
}

TEST(ReverseNumTests, HandlesPositiveNumbers) {
    int32_t result = 0;
    EXPECT_EQ(reverseNum(1234, &result), 0);
    EXPECT_EQ(result, 4321);
}

TEST(ReverseNumTests, HandlesNegativeNumbers) {
    int32_t result = 0;
    EXPECT_EQ(reverseNum(-1234, &result), 0);
    EXPECT_EQ(result, -4321);
}

TEST(ReverseNumTests, HandlesZero) {
    int32_t result = 0;
    EXPECT_EQ(reverseNum(0, &result), 0);
    EXPECT_EQ(result, 0);
}

TEST(ReverseNumTests, HandlesPositiveOverflow) {
    int32_t result = 0;
    // Reversing 1000000005 results in 5000000001, which is > INT32_MAX
    EXPECT_EQ(reverseNum(1000000005, &result), -1);
}

TEST(ReverseNumTests, HandlesNegativeOverflow) {
    int32_t result = 0;
    // Reversing -1000000009 results in -9000000001, which is < INT32_MIN
    EXPECT_EQ(reverseNum(-1000000009, &result), -1);
}

TEST(ReverseNumTests, HandlesInt32MaxBoundaries) {
    int32_t result = 0;
    // 2147483647 reversed is 7463847412 (Overflow)
    EXPECT_EQ(reverseNum(2147483647, &result), -1);
    
    // A number that reverses exactly to INT32_MAX (if possible)
    // 1463847412 reversed is 2147483641 (No overflow)
    EXPECT_EQ(reverseNum(1463847412, &result), 0);
    EXPECT_EQ(result, 2147483641);
}

TEST(ReverseNumTests, HandlesInt32MinBoundaries) {
    int32_t result = 0;
    // -2147483648 reversed is -8463847412 (Overflow)
    EXPECT_EQ(reverseNum(-2147483648, &result), -1);
}
