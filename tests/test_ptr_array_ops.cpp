#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {
    typedef enum {
        ARR_NULL = 0,
        ARR_ZERO_LEN,
        ARR_SUCCESS
    } arr_status_t;

    arr_status_t print_arr(const uint32_t* a, size_t len);
    arr_status_t rev_arr(uint32_t* a, size_t len);
}

TEST(PtrArrayOpsTest, PrintArrHandlesNullAndZeroLen) {
    uint32_t arr[3] = {10, 20, 30};
    EXPECT_EQ(print_arr(NULL, 3), ARR_NULL);
    EXPECT_EQ(print_arr(arr, 0), ARR_ZERO_LEN);
    EXPECT_EQ(print_arr(arr, 3), ARR_SUCCESS);
}

TEST(PtrArrayOpsTest, RevArrHandlesNullAndZeroLen) {
    uint32_t arr[3] = {10, 20, 30};
    EXPECT_EQ(rev_arr(NULL, 3), ARR_NULL);
    EXPECT_EQ(rev_arr(arr, 0), ARR_ZERO_LEN);
}

TEST(PtrArrayOpsTest, RevArrEvenLength) {
    uint32_t arr[4] = {1, 2, 3, 4};
    EXPECT_EQ(rev_arr(arr, 4), ARR_SUCCESS);
    EXPECT_EQ(arr[0], 4u);
    EXPECT_EQ(arr[1], 3u);
    EXPECT_EQ(arr[2], 2u);
    EXPECT_EQ(arr[3], 1u);
}

TEST(PtrArrayOpsTest, RevArrOddLength) {
    uint32_t arr[5] = {10, 20, 30, 40, 50};
    EXPECT_EQ(rev_arr(arr, 5), ARR_SUCCESS);
    EXPECT_EQ(arr[0], 50u);
    EXPECT_EQ(arr[1], 40u);
    EXPECT_EQ(arr[2], 30u);
    EXPECT_EQ(arr[3], 20u);
    EXPECT_EQ(arr[4], 10u);
}

TEST(PtrArrayOpsTest, RevArrSingleElement) {
    uint32_t arr[1] = {99};
    EXPECT_EQ(rev_arr(arr, 1), ARR_SUCCESS);
    EXPECT_EQ(arr[0], 99u);
}
