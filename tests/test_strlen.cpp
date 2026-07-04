#include <gtest/gtest.h>
#include <stddef.h>

extern "C" {
    size_t cus_strlen(const char* str);
}

TEST(StrLenTests, HandlesNull) {
    EXPECT_EQ(cus_strlen(NULL), 0);
}

TEST(StrLenTests, HandlesEmptyString) {
    EXPECT_EQ(cus_strlen(""), 0);
}

TEST(StrLenTests, HandlesNormalString) {
    EXPECT_EQ(cus_strlen("Hello"), 5);
}

TEST(StrLenTests, HandlesSpecialCharacters) {
    EXPECT_EQ(cus_strlen("Hello\nWorld!"), 12);
}
