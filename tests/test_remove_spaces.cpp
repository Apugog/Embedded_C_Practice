#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {
    typedef enum {
        SPACE_NULL = 0,
        SPACE_SUCCESS
    } space_status_t;

    space_status_t remove_space(char* str);
}

TEST(RemoveSpacesTest, HandlesNullPointer) {
    EXPECT_EQ(remove_space(NULL), SPACE_NULL);
}

TEST(RemoveSpacesTest, HandlesEmptyString) {
    char str[] = "";
    EXPECT_EQ(remove_space(str), SPACE_SUCCESS);
    EXPECT_STREQ(str, "");
}

TEST(RemoveSpacesTest, NoSpaces) {
    char str[] = "Hello";
    EXPECT_EQ(remove_space(str), SPACE_SUCCESS);
    EXPECT_STREQ(str, "Hello");
}

TEST(RemoveSpacesTest, MultipleSpacesInBetween) {
    char str[] = "H e l l o";
    EXPECT_EQ(remove_space(str), SPACE_SUCCESS);
    EXPECT_STREQ(str, "Hello");
}

TEST(RemoveSpacesTest, LeadingAndTrailingSpaces) {
    char str[] = "  Hello  ";
    EXPECT_EQ(remove_space(str), SPACE_SUCCESS);
    EXPECT_STREQ(str, "Hello");
}

TEST(RemoveSpacesTest, OnlySpaces) {
    char str[] = "     ";
    EXPECT_EQ(remove_space(str), SPACE_SUCCESS);
    EXPECT_STREQ(str, "");
}

TEST(RemoveSpacesTest, MixOfSpaces) {
    char str[] = " H e l l o   World ";
    EXPECT_EQ(remove_space(str), SPACE_SUCCESS);
    EXPECT_STREQ(str, "HelloWorld");
}
