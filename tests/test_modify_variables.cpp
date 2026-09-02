#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {
    typedef enum {
        STATUS_SUCCESS = 0,
        STATUS_NULL_PTR,
        STATUS_INVALID_PARAM
    } ptr_status_t;

    ptr_status_t compute_stats(const uint32_t *arr, size_t len, 
                               uint32_t *out_max, uint32_t *out_min, uint32_t *out_avg);
}

TEST(ModifyVariablesTest, NullPointerChecks) {
    uint32_t arr[3] = {1, 2, 3};
    uint32_t max = 0, min = 0, avg = 0;

    EXPECT_EQ(compute_stats(NULL, 3, &max, &min, &avg), STATUS_NULL_PTR);
    EXPECT_EQ(compute_stats(arr, 3, NULL, &min, &avg), STATUS_NULL_PTR);
    EXPECT_EQ(compute_stats(arr, 3, &max, NULL, &avg), STATUS_NULL_PTR);
    EXPECT_EQ(compute_stats(arr, 3, &max, &min, NULL), STATUS_NULL_PTR);
}

TEST(ModifyVariablesTest, ZeroLengthReturnsInvalidParam) {
    uint32_t arr[3] = {1, 2, 3};
    uint32_t max = 0, min = 0, avg = 0;

    EXPECT_EQ(compute_stats(arr, 0, &max, &min, &avg), STATUS_INVALID_PARAM);
}

TEST(ModifyVariablesTest, SingleElement) {
    uint32_t arr[1] = {42};
    uint32_t max = 0, min = 0, avg = 0;

    EXPECT_EQ(compute_stats(arr, 1, &max, &min, &avg), STATUS_SUCCESS);
    EXPECT_EQ(max, 42u);
    EXPECT_EQ(min, 42u);
    EXPECT_EQ(avg, 42u);
}

TEST(ModifyVariablesTest, MultipleElementsTypical) {
    uint32_t arr[5] = {1, 5, 3, 2, 4};
    uint32_t max = 0, min = 0, avg = 0;

    EXPECT_EQ(compute_stats(arr, 5, &max, &min, &avg), STATUS_SUCCESS);
    EXPECT_EQ(max, 5u);
    EXPECT_EQ(min, 1u);
    EXPECT_EQ(avg, 3u);
}

TEST(ModifyVariablesTest, IdenticalElements) {
    uint32_t arr[4] = {10, 10, 10, 10};
    uint32_t max = 0, min = 0, avg = 0;

    EXPECT_EQ(compute_stats(arr, 4, &max, &min, &avg), STATUS_SUCCESS);
    EXPECT_EQ(max, 10u);
    EXPECT_EQ(min, 10u);
    EXPECT_EQ(avg, 10u);
}

TEST(ModifyVariablesTest, LargeValuesAccumulatorOverflowCheck) {
    // Two large numbers that exceed UINT32_MAX when summed: 3,000,000,000 + 3,000,000,000 = 6,000,000,000
    uint32_t arr[2] = {3000000000U, 3000000000U};
    uint32_t max = 0, min = 0, avg = 0;

    EXPECT_EQ(compute_stats(arr, 2, &max, &min, &avg), STATUS_SUCCESS);
    EXPECT_EQ(max, 3000000000U);
    EXPECT_EQ(min, 3000000000U);
    EXPECT_EQ(avg, 3000000000U);
}

TEST(ModifyVariablesTest, BoundaryValues) {
    uint32_t arr[2] = {0, UINT32_MAX};
    uint32_t max = 0, min = 0, avg = 0;

    EXPECT_EQ(compute_stats(arr, 2, &max, &min, &avg), STATUS_SUCCESS);
    EXPECT_EQ(max, UINT32_MAX);
    EXPECT_EQ(min, 0u);
    EXPECT_EQ(avg, UINT32_MAX / 2u);
}
