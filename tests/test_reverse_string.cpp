#include <gtest/gtest.h>
#include <stdint.h>

extern "C" {
    typedef enum {
        REV_SUCCESS = 0,
        REV_NULL
    } rev_status_t;

    rev_status_t reverse_string(char* str);
}

TEST(ReverseStringTests, HandlesNull) {
    EXPECT_EQ(reverse_string(NULL), REV_NULL);
}

TEST(ReverseStringTests, HandlesEmptyString) {
    char str[] = "";
    EXPECT_EQ(reverse_string(str), REV_SUCCESS);
    EXPECT_STREQ(str, "");
}

TEST(ReverseStringTests, HandlesSingleCharacter) {
    char str[] = "A";
    EXPECT_EQ(reverse_string(str), REV_SUCCESS);
    EXPECT_STREQ(str, "A");
}

TEST(ReverseStringTests, HandlesEvenLength) {
    char str[] = "ABCD";
    EXPECT_EQ(reverse_string(str), REV_SUCCESS);
    EXPECT_STREQ(str, "DCBA");
}

TEST(ReverseStringTests, HandlesOddLength) {
    char str[] = "ABCDE";
    EXPECT_EQ(reverse_string(str), REV_SUCCESS);
    EXPECT_STREQ(str, "EDCBA");
}

TEST(ReverseStringTests, HandlesPalindrome) {
    char str[] = "radar";
    EXPECT_EQ(reverse_string(str), REV_SUCCESS);
    EXPECT_STREQ(str, "radar");
}
