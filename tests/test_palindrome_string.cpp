#include <gtest/gtest.h>
#include <stdint.h>

extern "C" {
    typedef enum {
        PALIN_FAIL = 0,
        PALIN_PASS,
        PALIN_NULL
    } palin_status_t;

    palin_status_t check_palindrome(const char* str);
}

TEST(PalindromeStringTests, HandlesNull) {
    EXPECT_EQ(check_palindrome(NULL), PALIN_NULL);
}

TEST(PalindromeStringTests, HandlesEmptyString) {
    EXPECT_EQ(check_palindrome(""), PALIN_PASS);
}

TEST(PalindromeStringTests, HandlesSingleCharacter) {
    EXPECT_EQ(check_palindrome("A"), PALIN_PASS);
}

TEST(PalindromeStringTests, HandlesOddLengthPalindrome) {
    EXPECT_EQ(check_palindrome("radar"), PALIN_PASS);
}

TEST(PalindromeStringTests, HandlesEvenLengthPalindrome) {
    EXPECT_EQ(check_palindrome("noon"), PALIN_PASS);
}

TEST(PalindromeStringTests, HandlesNonPalindrome) {
    EXPECT_EQ(check_palindrome("hello"), PALIN_FAIL);
}

TEST(PalindromeStringTests, CaseSensitivity) {
    // "Radar" is not a palindrome due to 'R' != 'r'
    EXPECT_EQ(check_palindrome("Radar"), PALIN_FAIL);
}
