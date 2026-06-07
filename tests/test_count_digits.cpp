#include <gtest/gtest.h>
#include <stdint.h>

extern "C" {
    uint8_t countNum(int32_t num);
}

TEST(CountNumTests, HandlesZero) {
    EXPECT_EQ(countNum(0), 1);
}

TEST(CountNumTests, HandlesPositiveSingleDigit) {
    EXPECT_EQ(countNum(5), 1);
}

TEST(CountNumTests, HandlesNegativeSingleDigit) {
    EXPECT_EQ(countNum(-5), 1);
}

TEST(CountNumTests, HandlesPositiveMultipleDigits) {
    EXPECT_EQ(countNum(12345), 5);
}

TEST(CountNumTests, HandlesNegativeMultipleDigits) {
    EXPECT_EQ(countNum(-12345), 5);
}

TEST(CountNumTests, HandlesInt32Max) {
    EXPECT_EQ(countNum(2147483647), 10);
}

TEST(CountNumTests, HandlesInt32Min) {
    EXPECT_EQ(countNum(-2147483648), 10);
}
