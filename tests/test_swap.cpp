#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {
    void swapNum(int32_t* a, int32_t* b);
}

TEST(SwapTests, HandlesStandardSwap) {
    int32_t a = 31;
    int32_t b = 10;
    swapNum(&a, &b);
    EXPECT_EQ(a, 10);
    EXPECT_EQ(b, 31);
}

TEST(SwapTests, HandlesNegativeNumbers) {
    int32_t a = -5;
    int32_t b = 25;
    swapNum(&a, &b);
    EXPECT_EQ(a, 25);
    EXPECT_EQ(b, -5);
}

TEST(SwapTests, HandlesLargeNumbersOverflow) {
    int32_t a = 2000000000;
    int32_t b = 2000000000;
    swapNum(&a, &b);
    EXPECT_EQ(a, 2000000000);
    EXPECT_EQ(b, 2000000000);
}

TEST(SwapTests, HandlesDifferentLargeNumbers) {
    int32_t a = 2000000000;
    int32_t b = -2000000000;
    swapNum(&a, &b);
    EXPECT_EQ(a, -2000000000);
    EXPECT_EQ(b, 2000000000);
}

TEST(SwapTests, HandlesAliasing) {
    int32_t x = 42;
    swapNum(&x, &x);
    EXPECT_EQ(x, 42); // Should remain 42
}

TEST(SwapTests, HandlesNullPointers) {
    int32_t x = 42;
    // Should not crash
    swapNum(&x, NULL);
    swapNum(NULL, &x);
    swapNum(NULL, NULL);
    EXPECT_EQ(x, 42);
}
