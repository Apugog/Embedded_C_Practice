#include <gtest/gtest.h>
#include <stdint.h>
#include <stdbool.h>

// Link the C implementation
extern "C" {
    bool isPrime(uint32_t n);
}

// Test Group: PrimeCheckTests
TEST(PrimeCheckTests, HandlesEdgeCases) {
    EXPECT_FALSE(isPrime(0));
    EXPECT_FALSE(isPrime(1));
}

TEST(PrimeCheckTests, HandlesSmallPrimes) {
    EXPECT_TRUE(isPrime(2));
    EXPECT_TRUE(isPrime(3));
    EXPECT_TRUE(isPrime(5));
    EXPECT_TRUE(isPrime(7));
    EXPECT_TRUE(isPrime(11));
    EXPECT_TRUE(isPrime(13));
}

TEST(PrimeCheckTests, HandlesSmallNonPrimes) {
    EXPECT_FALSE(isPrime(4));
    EXPECT_FALSE(isPrime(6));
    EXPECT_FALSE(isPrime(8));
    EXPECT_FALSE(isPrime(9));
    EXPECT_FALSE(isPrime(10));
    EXPECT_FALSE(isPrime(15));
}

TEST(PrimeCheckTests, HandlesLargePrimes) {
    // 104729 is the 10,000th prime
    EXPECT_TRUE(isPrime(104729));
    // 2147483647 is a Mersenne prime (2^31 - 1)
    // Note: If you use your O(n) implementation, this test will be VERY slow.
    EXPECT_TRUE(isPrime(2147483647));
}

TEST(PrimeCheckTests, HandlesLargeNonPrimes) {
    EXPECT_FALSE(isPrime(104728));
    EXPECT_FALSE(isPrime(2147483646));
}
