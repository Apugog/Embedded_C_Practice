#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define R 3
#define C 3
#define L (R*C)

typedef enum {
    MATRIX_SUCCESS = 0,
    MATRIX_ERR_NULL,
    MATRIX_ERR_EMPTY,
    MATRIX_ERR_OVERFLOW,
    MATRIX_ERR_OVERLAP
} matrix_status_t;

#define MAT_AT(mat, r, c, cols) ((mat)[(r)*(cols)+ (c)])

matrix_status_t matrix_add(const int32_t* a, const int32_t* b, int32_t* c, size_t row, size_t col)
{
    if (a == NULL || b == NULL || c == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (row == 0 || col == 0) {
        return MATRIX_ERR_EMPTY;
    }

    uintptr_t a_start = (uintptr_t)a;
    uintptr_t a_end = a_start + (row * col * sizeof(int32_t));
    uintptr_t b_start = (uintptr_t)b;
    uintptr_t b_end = b_start + (row * col * sizeof(int32_t));
    uintptr_t c_start = (uintptr_t)c;
    uintptr_t c_end = c_start + (row * col * sizeof(int32_t));

    // Check for unsafe overlapping buffers (excluding exact in-place match)
    if (a != c && a_start < c_end && c_start < a_end) {
        return MATRIX_ERR_OVERLAP;
    }
    if (b != c && b_start < c_end && c_start < b_end) {
        return MATRIX_ERR_OVERLAP;
    }

    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            int32_t val_a = MAT_AT(a, i, j, col);
            int32_t val_b = MAT_AT(b, i, j, col);

            // Safe signed integer overflow check
            if ((val_b > 0 && val_a > INT32_MAX - val_b) ||
                (val_b < 0 && val_a < INT32_MIN - val_b)) {
                return MATRIX_ERR_OVERFLOW;
            }

            MAT_AT(c, i, j, col) = val_a + val_b;
        }
    }

    return MATRIX_SUCCESS;
}

matrix_status_t matrix_transpose(const int32_t* a, int32_t* c, size_t row, size_t col)
{
    if (a == NULL || c == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (row == 0 || col == 0) {
        return MATRIX_ERR_EMPTY;
    }

    uintptr_t a_start = (uintptr_t)a;
    uintptr_t a_end = a_start + (row * col * sizeof(int32_t));
    uintptr_t c_start = (uintptr_t)c;
    uintptr_t c_end = c_start + (row * col * sizeof(int32_t));

    // In-place transpose is only allowed for square matrices
    if ((const int32_t*)c == a) {
        if (row != col) {
            return MATRIX_ERR_OVERLAP;
        }
        for (size_t i = 0; i < row; i++) {
            for (size_t j = i + 1; j < col; j++) {
                int32_t temp = MAT_AT(c, i, j, col);
                MAT_AT(c, i, j, col) = MAT_AT(c, j, i, col);
                MAT_AT(c, j, i, col) = temp;
            }
        }
        return MATRIX_SUCCESS;
    }

    // Overlapping buffers for different pointers
    if (a_start < c_end && c_start < a_end) {
        return MATRIX_ERR_OVERLAP;
    }

    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            MAT_AT(c, j, i, row) = MAT_AT(a, i, j, col);
        }
    }

    return MATRIX_SUCCESS;
}

matrix_status_t matrix_print(const int32_t* a, size_t row, size_t col)
{
    if (a == NULL) {
        return MATRIX_ERR_NULL;
    }

    if (row == 0 || col == 0) {
        return MATRIX_ERR_EMPTY;
    }

    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            printf("%3d%s", MAT_AT(a, i, j, col), (j == col - 1) ? "\n" : ", ");
        }
    }
    printf("\n");
    return MATRIX_SUCCESS;
}

#ifndef TESTING 
int main()
{
    int32_t a[R*C] = {1, 2, 3,
                      4, 5, 6,
                      7, 8, 9};
    int32_t b[R*C] = {1, 2, 3,
                      4, 5, 6,
                      7, 8, 9};
    int32_t c[R*C];

    matrix_print(a, R, C);
    matrix_transpose(a, c, R, C);
    matrix_print(c, R, C);

    return 0;
}
#endif