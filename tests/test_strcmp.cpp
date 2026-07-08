#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {
    typedef enum {
        STRCMP_SUCCESS,
        STRCMP_NULL_PTR
    } strcmp_status_t;

    strcmp_status_t custom_setup(const char* s1, const char* s2, int32_t* result);
}

TEST(StrCmpTests, HandlesNullS1) {
    int32_t result = 999;
    EXPECT_EQ(custom_setup(NULL, "test", &result), STRCMP_NULL_PTR);
    EXPECT_EQ(result, 999); // Result should not be modified
}

TEST(StrCmpTests, HandlesNullS2) {
    int32_t result = 999;
    EXPECT_EQ(custom_setup("test", NULL, &result), STRCMP_NULL_PTR);
    EXPECT_EQ(result, 999);
}

TEST(StrCmpTests, HandlesNullResult) {
    EXPECT_EQ(custom_setup("test", "test", NULL), STRCMP_NULL_PTR);
}

TEST(StrCmpTests, HandlesBothEmptyStrings) {
    int32_t result = 999;
    EXPECT_EQ(custom_setup("", "", &result), STRCMP_SUCCESS);
    EXPECT_EQ(result, 0);
}

TEST(StrCmpTests, HandlesIdenticalStrings) {
    int32_t result = 999;
    EXPECT_EQ(custom_setup("Hello, World!", "Hello, World!", &result), STRCMP_SUCCESS);
    EXPECT_EQ(result, 0);
}

TEST(StrCmpTests, HandlesS1LessThanS2) {
    int32_t result = 999;
    EXPECT_EQ(custom_setup("apple", "banana", &result), STRCMP_SUCCESS);
    EXPECT_EQ(result, -1);
}

TEST(StrCmpTests, HandlesS1GreaterThanS2) {
    int32_t result = 999;
    EXPECT_EQ(custom_setup("banana", "apple", &result), STRCMP_SUCCESS);
    EXPECT_EQ(result, 1);
}

TEST(StrCmpTests, HandlesS1IsPrefixOfS2) {
    int32_t result = 999;
    EXPECT_EQ(custom_setup("cat", "caterpillar", &result), STRCMP_SUCCESS);
    EXPECT_EQ(result, -1);
}

TEST(StrCmpTests, HandlesS2IsPrefixOfS1) {
    int32_t result = 999;
    EXPECT_EQ(custom_setup("caterpillar", "cat", &result), STRCMP_SUCCESS);
    EXPECT_EQ(result, 1);
}

TEST(StrCmpTests, HandlesCaseSensitivity) {
    int32_t result = 999;
    EXPECT_EQ(custom_setup("cat", "Cat", &result), STRCMP_SUCCESS);
    EXPECT_EQ(result, 1); // 'c' (97) > 'C' (65)
}

TEST(StrCmpTests, HandlesSignedUnsignedCharacters) {
    int32_t result = 999;
    // Character with value 255 (\xff) vs value 1 (\x01)
    EXPECT_EQ(custom_setup("\xff", "\x01", &result), STRCMP_SUCCESS);
    EXPECT_EQ(result, 1); // 255 > 1, so s1 > s2
}
