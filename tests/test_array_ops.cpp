#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {
    typedef enum {
        ARRAY_SUCCESS = 0,
        ARRAY_ERR_NULL,
        ARRAY_ERR_EMPTY,
        ARRAY_ERR_NO_SECOND_LARGEST
    } array_status_t;

    int32_t largest(const int32_t* arr, size_t len, int32_t* result);
    int32_t smallest(const int32_t* arr, size_t len, int32_t* result);
    int32_t second_largest(int32_t* arr, size_t len, int32_t* result);
}

// Largest Tests
TEST(ArrayLargestTests, StandardArray) {
    int32_t arr[] = {12, 35, 1, 10, 34, 1};
    int32_t result = 0;
    EXPECT_EQ(largest(arr, 6, &result), ARRAY_SUCCESS);
    EXPECT_EQ(result, 35);
}

TEST(ArrayLargestTests, NegativesAndPositives) {
    int32_t arr[] = {-10, -20, 0, -5, 5};
    int32_t result = 0;
    EXPECT_EQ(largest(arr, 5, &result), ARRAY_SUCCESS);
    EXPECT_EQ(result, 5);
}

TEST(ArrayLargestTests, SingleElement) {
    int32_t arr[] = {42};
    int32_t result = 0;
    EXPECT_EQ(largest(arr, 1, &result), ARRAY_SUCCESS);
    EXPECT_EQ(result, 42);
}

TEST(ArrayLargestTests, NullOrEmpty) {
    int32_t result = 0;
    EXPECT_EQ(largest(NULL, 5, &result), ARRAY_ERR_NULL);
    int32_t arr[] = {1};
    EXPECT_EQ(largest(arr, 0, &result), ARRAY_ERR_EMPTY);
}

// Smallest Tests
TEST(ArraySmallestTests, StandardArray) {
    int32_t arr[] = {12, 35, 1, 10, 34, 1};
    int32_t result = 0;
    EXPECT_EQ(smallest(arr, 6, &result), ARRAY_SUCCESS);
    EXPECT_EQ(result, 1);
}

TEST(ArraySmallestTests, NegativesAndPositives) {
    int32_t arr[] = {-10, -20, 0, -5, 5};
    int32_t result = 0;
    EXPECT_EQ(smallest(arr, 5, &result), ARRAY_SUCCESS);
    EXPECT_EQ(result, -20);
}

TEST(ArraySmallestTests, NullOrEmpty) {
    int32_t result = 0;
    EXPECT_EQ(smallest(NULL, 5, &result), ARRAY_ERR_NULL);
    int32_t arr[] = {1};
    EXPECT_EQ(smallest(arr, 0, &result), ARRAY_ERR_EMPTY);
}

// Second Largest Tests
TEST(ArraySecondLargestTests, StandardArray) {
    int32_t arr[] = {12, 35, 1, 10, 34, 1};
    int32_t result = 0;
    EXPECT_EQ(second_largest(arr, 6, &result), ARRAY_SUCCESS);
    EXPECT_EQ(result, 34);
}

TEST(ArraySecondLargestTests, DuplicateMax) {
    int32_t arr[] = {35, 35, 10, 20};
    int32_t result = 0;
    EXPECT_EQ(second_largest(arr, 4, &result), ARRAY_SUCCESS);
    EXPECT_EQ(result, 20);
}

TEST(ArraySecondLargestTests, AllDuplicates) {
    int32_t arr[] = {10, 10, 10};
    int32_t result = 0;
    EXPECT_EQ(second_largest(arr, 3, &result), ARRAY_ERR_NO_SECOND_LARGEST);
}

TEST(ArraySecondLargestTests, SingleElement) {
    int32_t arr[] = {42};
    int32_t result = 0;
    EXPECT_EQ(second_largest(arr, 1, &result), ARRAY_ERR_NO_SECOND_LARGEST);
}

TEST(ArraySecondLargestTests, BoundaryMin) {
    int32_t arr[] = {10, INT32_MIN};
    int32_t result = 0;
    EXPECT_EQ(second_largest(arr, 2, &result), ARRAY_SUCCESS);
    EXPECT_EQ(result, INT32_MIN);
}
