#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

typedef enum {
    SWAP_OK,
    SWAP_NULL
} swap_status_t;

extern "C" {
    swap_status_t swap_xor(uint32_t *a, uint32_t *b);
}

TEST(SwapXorTest, StandardSwap) {
    uint32_t a = 10;
    uint32_t b = 20;
    EXPECT_EQ(swap_xor(&a, &b), SWAP_OK);
    EXPECT_EQ(a, 20U);
    EXPECT_EQ(b, 10U);
}

TEST(SwapXorTest, EqualValuesDifferentAddresses) {
    uint32_t a = 42;
    uint32_t b = 42;
    EXPECT_EQ(swap_xor(&a, &b), SWAP_OK);
    EXPECT_EQ(a, 42U);
    EXPECT_EQ(b, 42U);
}

TEST(SwapXorTest, SelfAliasingSameAddress) {
    uint32_t a = 0xDEADBEEFU;
    EXPECT_EQ(swap_xor(&a, &a), SWAP_OK);
    EXPECT_EQ(a, 0xDEADBEEFU);
}

TEST(SwapXorTest, BoundaryValuesExtremes) {
    uint32_t a = 0x00000000U;
    uint32_t b = 0xFFFFFFFFU;
    EXPECT_EQ(swap_xor(&a, &b), SWAP_OK);
    EXPECT_EQ(a, 0xFFFFFFFFU);
    EXPECT_EQ(b, 0x00000000U);
}

TEST(SwapXorTest, NullPointerGuards) {
    uint32_t a = 100;
    EXPECT_EQ(swap_xor(NULL, &a), SWAP_NULL);
    EXPECT_EQ(swap_xor(&a, NULL), SWAP_NULL);
    EXPECT_EQ(swap_xor(NULL, NULL), SWAP_NULL);
    EXPECT_EQ(a, 100U);
}

TEST(SwapXorTest, AlternatingBitPatterns) {
    uint32_t a = 0xAAAAAAAAU;
    uint32_t b = 0x55555555U;
    EXPECT_EQ(swap_xor(&a, &b), SWAP_OK);
    EXPECT_EQ(a, 0x55555555U);
    EXPECT_EQ(b, 0xAAAAAAAAU);
}
