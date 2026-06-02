#include <gtest/gtest.h>
#include "../1_Basic_C_Programming/1.1_Fibonacci_series.h"

// Note: Since print_fibonacci prints to stdout, testing it directly 
// usually involves capturing stdout. For this basic example, 
// we just check if it runs without crashing.
// In a real scenario, you'd refactor the function to return a value 
// or fill a buffer.

TEST(FibonacciTest, HandlesZeroInput) {
    // This just verifies the function can be called.
    // Ideally, we'd capture stdout and verify the message.
    print_fibonacci(0);
}

TEST(FibonacciTest, HandlesSmallInput) {
    print_fibonacci(5);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
