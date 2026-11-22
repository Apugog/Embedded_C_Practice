#include <gtest/gtest.h>
#include <stdint.h>

extern "C" {
    int32_t fibonacci_nth_recursive(int32_t n);
    void fibonacci(uint32_t num);
}

// Suite 1: Base Cases (F(0) = 0, F(1) = 1, F(2) = 1)
TEST(FibonacciRecursionTest, BaseCases) {
    EXPECT_EQ(fibonacci_nth_recursive(0), 0);
    EXPECT_EQ(fibonacci_nth_recursive(1), 1);
    EXPECT_EQ(fibonacci_nth_recursive(2), 1);
}

// Suite 2: Small Positive Integers (Tree Recursion Verification)
TEST(FibonacciRecursionTest, SmallPositiveNumbers) {
    EXPECT_EQ(fibonacci_nth_recursive(3), 2);
    EXPECT_EQ(fibonacci_nth_recursive(4), 3);
    EXPECT_EQ(fibonacci_nth_recursive(5), 5);
    EXPECT_EQ(fibonacci_nth_recursive(6), 8);
    EXPECT_EQ(fibonacci_nth_recursive(7), 13);
    EXPECT_EQ(fibonacci_nth_recursive(8), 21);
    EXPECT_EQ(fibonacci_nth_recursive(9), 34);
    EXPECT_EQ(fibonacci_nth_recursive(10), 55);
    EXPECT_EQ(fibonacci_nth_recursive(12), 144);
    EXPECT_EQ(fibonacci_nth_recursive(15), 610);
    EXPECT_EQ(fibonacci_nth_recursive(18), 2584);
}

// Suite 3: Negative Invalid Inputs (n < 0)
TEST(FibonacciRecursionTest, NegativeInputs) {
    EXPECT_EQ(fibonacci_nth_recursive(-1), -1);
    EXPECT_EQ(fibonacci_nth_recursive(-5), -1);
    EXPECT_EQ(fibonacci_nth_recursive(-100), -1);
    EXPECT_EQ(fibonacci_nth_recursive(INT32_MIN), -1);
}

// Suite 4: Signed 32-bit Overflow Protection (n > 46)
// Verifies O(1) early rejection before exponential O(2^n) call stack expansion
TEST(FibonacciRecursionTest, OverflowProtection) {
    EXPECT_EQ(fibonacci_nth_recursive(47), -1);
    EXPECT_EQ(fibonacci_nth_recursive(48), -1);
    EXPECT_EQ(fibonacci_nth_recursive(50), -1);
    EXPECT_EQ(fibonacci_nth_recursive(100), -1);
    EXPECT_EQ(fibonacci_nth_recursive(100000), -1);
    EXPECT_EQ(fibonacci_nth_recursive(INT32_MAX), -1);
}

// Suite 5: Linear Tail-Recursive Series Generator Interface
TEST(FibonacciRecursionTest, SeriesGeneratorBoundaries) {
    // Edge case: Count 0 is rejected gracefully without crashing
    fibonacci(0);

    // Single term boundary
    fibonacci(1);

    // Standard sequence generation
    fibonacci(10);

    // Maximum 32-bit term boundary
    fibonacci(46);

    // Overflow boundary rejection (> 46)
    fibonacci(47);
    fibonacci(100);
}
