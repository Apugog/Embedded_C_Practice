#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {
    typedef enum {
        PTR_STATUS_SUCCESS = 0,
        PTR_STATUS_NULL_PTR,
        PTR_STATUS_EMPTY
    } ptr_status_t;

    ptr_status_t print_elements(const uint32_t *arr, size_t len);
}

TEST(PrintElementsTest, NullPointerReturnsNullStatus) {
    EXPECT_EQ(print_elements(NULL, 5), PTR_STATUS_NULL_PTR);
    EXPECT_EQ(print_elements(NULL, 0), PTR_STATUS_NULL_PTR);
}

TEST(PrintElementsTest, ZeroLengthReturnsEmptyStatus) {
    uint32_t arr[3] = {1, 2, 3};
    EXPECT_EQ(print_elements(arr, 0), PTR_STATUS_EMPTY);
}

TEST(PrintElementsTest, SingleElementSuccess) {
    uint32_t arr[1] = {42};
    EXPECT_EQ(print_elements(arr, 1), PTR_STATUS_SUCCESS);
}

TEST(PrintElementsTest, MultipleElementsSuccess) {
    uint32_t arr[5] = {10, 20, 30, 40, 50};
    EXPECT_EQ(print_elements(arr, 5), PTR_STATUS_SUCCESS);
}

TEST(PrintElementsTest, BoundaryValuesSuccess) {
    uint32_t arr[4] = {0, UINT32_MAX, 1, UINT32_MAX - 1};
    EXPECT_EQ(print_elements(arr, 4), PTR_STATUS_SUCCESS);
}
