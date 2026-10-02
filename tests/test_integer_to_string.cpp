#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {
    typedef enum {
        ITOA_OK = 0,
        ITOA_ERR_NULL_PTR,
        ITOA_ERR_BUFFER_TOO_SMALL
    } itoa_status_t;

    itoa_status_t int32_to_str(int32_t value, char *buf, size_t buf_size);
    itoa_status_t int32_to_str_stdlib(int32_t value, char *buf, size_t buf_size);
}

TEST(IntegerToStringTests, RejectsNullPointer) {
    EXPECT_EQ(int32_to_str(123, NULL, 16), ITOA_ERR_NULL_PTR);
}

TEST(IntegerToStringTests, RejectsZeroBufferSize) {
    char buf[16];
    EXPECT_EQ(int32_to_str(123, buf, 0), ITOA_ERR_BUFFER_TOO_SMALL);
}

TEST(IntegerToStringTests, ConvertsZero) {
    char buf[16];
    EXPECT_EQ(int32_to_str(0, buf, sizeof(buf)), ITOA_OK);
    EXPECT_STREQ(buf, "0");
}

TEST(IntegerToStringTests, ConvertsPositiveSingleDigit) {
    char buf[16];
    EXPECT_EQ(int32_to_str(7, buf, sizeof(buf)), ITOA_OK);
    EXPECT_STREQ(buf, "7");
}

TEST(IntegerToStringTests, ConvertsPositiveMultiDigit) {
    char buf[16];
    EXPECT_EQ(int32_to_str(12345, buf, sizeof(buf)), ITOA_OK);
    EXPECT_STREQ(buf, "12345");
}

TEST(IntegerToStringTests, ConvertsNegativeSingleDigit) {
    char buf[16];
    EXPECT_EQ(int32_to_str(-9, buf, sizeof(buf)), ITOA_OK);
    EXPECT_STREQ(buf, "-9");
}

TEST(IntegerToStringTests, ConvertsNegativeMultiDigit) {
    char buf[16];
    EXPECT_EQ(int32_to_str(-98765, buf, sizeof(buf)), ITOA_OK);
    EXPECT_STREQ(buf, "-98765");
}

TEST(IntegerToStringTests, ConvertsInt32Max) {
    char buf[16];
    EXPECT_EQ(int32_to_str(2147483647, buf, sizeof(buf)), ITOA_OK);
    EXPECT_STREQ(buf, "2147483647");
}

TEST(IntegerToStringTests, ConvertsInt32MinSafely) {
    char buf[16];
    /* -2147483647 - 1 is INT32_MIN (-2147483648) */
    EXPECT_EQ(int32_to_str(-2147483647 - 1, buf, sizeof(buf)), ITOA_OK);
    EXPECT_STREQ(buf, "-2147483648");
}

TEST(IntegerToStringTests, FailsWhenBufferTooSmallForPositive) {
    char small_buf[5]; // "12345" needs 6 bytes (including '\0')
    EXPECT_EQ(int32_to_str(12345, small_buf, sizeof(small_buf)), ITOA_ERR_BUFFER_TOO_SMALL);
}

TEST(IntegerToStringTests, FailsWhenBufferTooSmallForNegative) {
    char small_buf[6]; // "-12345" needs 7 bytes (including '\0')
    EXPECT_EQ(int32_to_str(-12345, small_buf, sizeof(small_buf)), ITOA_ERR_BUFFER_TOO_SMALL);
}

TEST(IntegerToStringTests, HandlesExactFitBufferSize) {
    char exact_buf[12]; // "-2147483648" needs exactly 12 bytes
    EXPECT_EQ(int32_to_str(-2147483647 - 1, exact_buf, sizeof(exact_buf)), ITOA_OK);
    EXPECT_STREQ(exact_buf, "-2147483648");
}

TEST(IntegerToStringTests, FailsWhenBufferIsOffByOneForInt32Min) {
    char off_by_one_buf[11]; // Needs 12 bytes, given 11
    EXPECT_EQ(int32_to_str(-2147483647 - 1, off_by_one_buf, sizeof(off_by_one_buf)), ITOA_ERR_BUFFER_TOO_SMALL);
}

/* ==================== Standard Library (snprintf) Tests ==================== */

TEST(IntegerToStringStdLibTests, RejectsNullPointer) {
    EXPECT_EQ(int32_to_str_stdlib(123, NULL, 16), ITOA_ERR_NULL_PTR);
}

TEST(IntegerToStringStdLibTests, RejectsZeroBufferSize) {
    char buf[16];
    EXPECT_EQ(int32_to_str_stdlib(123, buf, 0), ITOA_ERR_BUFFER_TOO_SMALL);
}

TEST(IntegerToStringStdLibTests, ConvertsZero) {
    char buf[16];
    EXPECT_EQ(int32_to_str_stdlib(0, buf, sizeof(buf)), ITOA_OK);
    EXPECT_STREQ(buf, "0");
}

TEST(IntegerToStringStdLibTests, ConvertsPositiveNumber) {
    char buf[16];
    EXPECT_EQ(int32_to_str_stdlib(12345, buf, sizeof(buf)), ITOA_OK);
    EXPECT_STREQ(buf, "12345");
}

TEST(IntegerToStringStdLibTests, ConvertsNegativeNumber) {
    char buf[16];
    EXPECT_EQ(int32_to_str_stdlib(-98765, buf, sizeof(buf)), ITOA_OK);
    EXPECT_STREQ(buf, "-98765");
}

TEST(IntegerToStringStdLibTests, ConvertsInt32MinSafely) {
    char buf[16];
    EXPECT_EQ(int32_to_str_stdlib(-2147483647 - 1, buf, sizeof(buf)), ITOA_OK);
    EXPECT_STREQ(buf, "-2147483648");
}

TEST(IntegerToStringStdLibTests, FailsWhenBufferTooSmall) {
    char small_buf[5]; // "12345" needs 6 bytes
    EXPECT_EQ(int32_to_str_stdlib(12345, small_buf, sizeof(small_buf)), ITOA_ERR_BUFFER_TOO_SMALL);
}

TEST(IntegerToStringStdLibTests, HandlesExactFitBufferSize) {
    char exact_buf[12]; // "-2147483648" needs 12 bytes
    EXPECT_EQ(int32_to_str_stdlib(-2147483647 - 1, exact_buf, sizeof(exact_buf)), ITOA_OK);
    EXPECT_STREQ(exact_buf, "-2147483648");
}

