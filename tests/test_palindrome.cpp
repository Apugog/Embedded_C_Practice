#include <gtest/gtest.h>
#include <stdint.h>
#include <stdbool.h>

extern "C" {
    bool isPalindrome(int32_t num);
}

TEST(PalindromeTests, HandlesZeroAndSingleDigits) {
    EXPECT_TRUE(isPalindrome(0));
    EXPECT_TRUE(isPalindrome(7));
}

TEST(PalindromeTests, HandlesEvenLengthPalindromes) {
    EXPECT_TRUE(isPalindrome(11));
    EXPECT_TRUE(isPalindrome(1221));
    EXPECT_TRUE(isPalindrome(123321));
}

TEST(PalindromeTests, HandlesOddLengthPalindromes) {
    EXPECT_TRUE(isPalindrome(121));
    EXPECT_TRUE(isPalindrome(12321));
}

TEST(PalindromeTests, HandlesNegativeNumbers) {
    EXPECT_FALSE(isPalindrome(-121));
    EXPECT_FALSE(isPalindrome(-11));
    EXPECT_FALSE(isPalindrome(-7));
}

TEST(PalindromeTests, HandlesNumbersEndingWithZero) {
    EXPECT_FALSE(isPalindrome(10));
    EXPECT_FALSE(isPalindrome(100));
    EXPECT_FALSE(isPalindrome(120));
}

TEST(PalindromeTests, HandlesNonPalindromes) {
    EXPECT_FALSE(isPalindrome(12));
    EXPECT_FALSE(isPalindrome(123));
    EXPECT_FALSE(isPalindrome(1234));
}

TEST(PalindromeTests, HandlesLargeNumbers) {
    EXPECT_TRUE(isPalindrome(123454321));
    EXPECT_FALSE(isPalindrome(INT32_MAX));
}
