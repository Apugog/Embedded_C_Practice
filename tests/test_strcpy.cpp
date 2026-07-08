#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {
    typedef enum {
        STRCPY_SUCCESS = 0,
        STRCPY_NULL_PTR
    } strcpy_status_t;

    strcpy_status_t custom_strcpy(char* dest, const char* src);
}

TEST(StrCpyTests, HandlesNullDest) {
    EXPECT_EQ(custom_strcpy(NULL, "test"), STRCPY_NULL_PTR);
}

TEST(StrCpyTests, HandlesNullSrc) {
    char dest[10];
    EXPECT_EQ(custom_strcpy(dest, NULL), STRCPY_NULL_PTR);
}

TEST(StrCpyTests, SimpleCopy) {
    char dest[10] = {0};
    const char* src = "Hello";
    EXPECT_EQ(custom_strcpy(dest, src), STRCPY_SUCCESS);
    EXPECT_STREQ(dest, "Hello");
}

TEST(StrCpyTests, OverlapDestAfterSrc) {
    char buf[20] = "abcdef";
    // src = "abcdef" (starts at index 0)
    // dest = starts at index 2 ("cdef")
    // Expected result in buf: "ababcdef"
    EXPECT_EQ(custom_strcpy(buf + 2, buf), STRCPY_SUCCESS);
    EXPECT_STREQ(buf, "ababcdef");
}

TEST(StrCpyTests, OverlapDestBeforeSrc) {
    char buf[20] = "abcdef";
    // src = "cdef" (starts at index 2)
    // dest = starts at index 0
    // Expected result: "cdef" starting at index 0
    EXPECT_EQ(custom_strcpy(buf, buf + 2), STRCPY_SUCCESS);
    EXPECT_STREQ(buf, "cdef");
}
