#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {
    typedef enum {
        MATRIX_SUCCESS = 0,
        MATRIX_ERR_NULL,
        MATRIX_ERR_EMPTY,
        MATRIX_ERR_OVERFLOW,
        MATRIX_ERR_OVERLAP
    } matrix_status_t;

    matrix_status_t matrix_add(const int32_t* a, const int32_t* b, int32_t* c, size_t row, size_t col);
    matrix_status_t matrix_transpose(const int32_t* a, int32_t* c, size_t row, size_t col);
}

// Matrix Addition Tests
TEST(MatrixAddTests, StandardAddition) {
    int32_t a[6] = {1, 2, 3, 4, 5, 6};
    int32_t b[6] = {10, 20, 30, 40, 50, 60};
    int32_t c[6] = {0};
    EXPECT_EQ(matrix_add(a, b, c, 2, 3), MATRIX_SUCCESS);
    EXPECT_EQ(c[0], 11);
    EXPECT_EQ(c[1], 22);
    EXPECT_EQ(c[2], 33);
    EXPECT_EQ(c[3], 44);
    EXPECT_EQ(c[4], 55);
    EXPECT_EQ(c[5], 66);
}

TEST(MatrixAddTests, InPlaceAddition) {
    int32_t a[6] = {1, 2, 3, 4, 5, 6};
    int32_t b[6] = {10, 20, 30, 40, 50, 60};
    EXPECT_EQ(matrix_add(a, b, a, 2, 3), MATRIX_SUCCESS);
    EXPECT_EQ(a[0], 11);
    EXPECT_EQ(a[5], 66);
}

TEST(MatrixAddTests, NullAndEmptyCheck) {
    int32_t a[4] = {1, 2, 3, 4};
    int32_t b[4] = {1, 2, 3, 4};
    int32_t c[4] = {0};
    EXPECT_EQ(matrix_add(NULL, b, c, 2, 2), MATRIX_ERR_NULL);
    EXPECT_EQ(matrix_add(a, NULL, c, 2, 2), MATRIX_ERR_NULL);
    EXPECT_EQ(matrix_add(a, b, NULL, 2, 2), MATRIX_ERR_NULL);
    EXPECT_EQ(matrix_add(a, b, c, 0, 2), MATRIX_ERR_EMPTY);
    EXPECT_EQ(matrix_add(a, b, c, 2, 0), MATRIX_ERR_EMPTY);
}

TEST(MatrixAddTests, OverflowCheck) {
    int32_t a[1] = {INT32_MAX};
    int32_t b[1] = {1};
    int32_t c[1] = {0};
    EXPECT_EQ(matrix_add(a, b, c, 1, 1), MATRIX_ERR_OVERFLOW);

    int32_t a2[1] = {INT32_MIN};
    int32_t b2[1] = {-1};
    EXPECT_EQ(matrix_add(a2, b2, c, 1, 1), MATRIX_ERR_OVERFLOW);
}

TEST(MatrixAddTests, OverlapCheck) {
    int32_t a[4] = {1, 2, 3, 4};
    // Sub-offset overlap: c starts at a + 1
    int32_t* c = a + 1;
    EXPECT_EQ(matrix_add(a, a, c, 1, 2), MATRIX_ERR_OVERLAP);
}

// Matrix Transpose Tests
TEST(MatrixTransposeTests, RectangularTranspose) {
    // 2x3 matrix:
    // 1, 2, 3
    // 4, 5, 6
    int32_t a[6] = {1, 2, 3, 4, 5, 6};
    // Transposed should be 3x2:
    // 1, 4
    // 2, 5
    // 3, 6
    int32_t c[6] = {0};
    EXPECT_EQ(matrix_transpose(a, c, 2, 3), MATRIX_SUCCESS);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 4);
    EXPECT_EQ(c[2], 2);
    EXPECT_EQ(c[3], 5);
    EXPECT_EQ(c[4], 3);
    EXPECT_EQ(c[5], 6);
}

TEST(MatrixTransposeTests, SquareInPlaceTranspose) {
    // 3x3 matrix:
    // 1, 2, 3
    // 4, 5, 6
    // 7, 8, 9
    int32_t a[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    EXPECT_EQ(matrix_transpose(a, a, 3, 3), MATRIX_SUCCESS);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 4);
    EXPECT_EQ(a[2], 7);
    EXPECT_EQ(a[3], 2);
    EXPECT_EQ(a[4], 5);
    EXPECT_EQ(a[5], 8);
    EXPECT_EQ(a[6], 3);
    EXPECT_EQ(a[7], 6);
    EXPECT_EQ(a[8], 9);
}

TEST(MatrixTransposeTests, NonSquareInPlaceFails) {
    int32_t a[6] = {1, 2, 3, 4, 5, 6};
    EXPECT_EQ(matrix_transpose(a, a, 2, 3), MATRIX_ERR_OVERLAP);
}

TEST(MatrixTransposeTests, NullAndEmptyCheck) {
    int32_t a[4] = {1, 2, 3, 4};
    int32_t c[4] = {0};
    EXPECT_EQ(matrix_transpose(NULL, c, 2, 2), MATRIX_ERR_NULL);
    EXPECT_EQ(matrix_transpose(a, NULL, 2, 2), MATRIX_ERR_NULL);
    EXPECT_EQ(matrix_transpose(a, c, 0, 2), MATRIX_ERR_EMPTY);
    EXPECT_EQ(matrix_transpose(a, c, 2, 0), MATRIX_ERR_EMPTY);
}

TEST(MatrixTransposeTests, OverlapCheck) {
    int32_t a[4] = {1, 2, 3, 4};
    int32_t* c = a + 1;
    EXPECT_EQ(matrix_transpose(a, c, 1, 2), MATRIX_ERR_OVERLAP);
}
