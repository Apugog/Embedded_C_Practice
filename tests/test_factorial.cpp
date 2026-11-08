#include <gtest/gtest.h>
#include <stdint.h>

extern "C" {
    int fact(int n);
}

// Suite 1: Base Cases (0! and 1!)
TEST(FactorialTest, BaseCases) {
    EXPECT_EQ(fact(0), 1);
    EXPECT_EQ(fact(1), 1);
}

// Suite 2: Small Positive Integers
TEST(FactorialTest, SmallPositiveNumbers) {
    EXPECT_EQ(fact(2), 2);
    EXPECT_EQ(fact(3), 6);
    EXPECT_EQ(fact(4), 24);
    EXPECT_EQ(fact(5), 120);
    EXPECT_EQ(fact(6), 720);
    EXPECT_EQ(fact(7), 5040);
}

// Suite 3: Maximum 32-bit Valid Input Boundary
TEST(FactorialTest, MaxBoundaryValues) {
    EXPECT_EQ(fact(10), 3628800);
    EXPECT_EQ(fact(11), 39916800);
    EXPECT_EQ(fact(12), 479001600); // Max valid factorial fitting in signed 32-bit int
}

// Suite 4: Overflow Protection (n > 12)
TEST(FactorialTest, OverflowProtection) {
    EXPECT_EQ(fact(13), -1);
    EXPECT_EQ(fact(14), -1);
    EXPECT_EQ(fact(20), -1);
    EXPECT_EQ(fact(100), -1);
    EXPECT_EQ(fact(100000), -1); // Verifies no recursion/stack overflow on massive input
}

// Suite 5: Negative Invalid Inputs (n < 0)
TEST(FactorialTest, NegativeInputs) {
    EXPECT_EQ(fact(-1), -1);
    EXPECT_EQ(fact(-5), -1);
    EXPECT_EQ(fact(-100), -1);
    EXPECT_EQ(fact(INT32_MIN), -1);
}
