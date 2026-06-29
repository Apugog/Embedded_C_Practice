#include <stdio.h>
#include <stdint.h>

#include <stddef.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef enum
{
    ARRAY_SUCCESS = 0,
    ARRAY_ERR_NULL,
    ARRAY_ERR_EMPTY,
    ARRAY_ERR_INSUFFICIENT_CAPACITY
} array_status_t;

array_status_t merge_sorted_arrays(const int32_t *arr1, size_t len1,
                                   const int32_t *arr2, size_t len2,
                                   int32_t *dest, size_t dest_capacity)
{
    // 1. Null pointer safety checks
    if (arr1 == NULL || arr2 == NULL || dest == NULL)
    {
        return ARRAY_ERR_NULL;
    }

    // 2. Empty array check (both must not be empty)
    if (len1 == 0 && len2 == 0)
    {
        return ARRAY_ERR_EMPTY;
    }

    // 3. Overflow validation (len1 + len2 can overflow size_t in extreme cases)
    if (SIZE_MAX - len1 < len2)
    {
        return ARRAY_ERR_INSUFFICIENT_CAPACITY;
    }

    // 4. Capacity validation (destination must be large enough)
    size_t required_capacity = len1 + len2;
    if (dest_capacity < required_capacity)
    {
        return ARRAY_ERR_INSUFFICIENT_CAPACITY;
    }

    size_t idx_1 = 0;
    size_t idx_2 = 0;
    size_t idx_d = 0;

    // 5. Merge loop
    while (idx_1 < len1 && idx_2 < len2)
    {
        if (arr1[idx_1] <= arr2[idx_2])
        {
            dest[idx_d++] = arr1[idx_1++];
        }
        else
        {
            dest[idx_d++] = arr2[idx_2++];
        }
    }

    // 6. Copy remaining elements
    while (idx_1 < len1)
    {
        dest[idx_d++] = arr1[idx_1++];
    }

    while (idx_2 < len2)
    {
        dest[idx_d++] = arr2[idx_2++];
    }

    return ARRAY_SUCCESS;
}

static void print_arr(const int32_t *arr, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

#ifndef TESTING
int main()
{
    int32_t arr1[] = {1, 3, 5, 7};
    int32_t arr2[] = {2, 4, 6, 8, 10};
    int32_t dest[9];

    printf("Array 1: ");
    print_arr(arr1, 4);
    printf("Array 2: ");
    print_arr(arr2, 5);

    array_status_t status = merge_sorted_arrays(arr1, 4, arr2, 5, dest, 9);
    if (status == ARRAY_SUCCESS)
    {
        printf("Merged: ");
        print_arr(dest, 9);
    }
    else
    {
        printf("Error merging arrays: %d\n", status);
    }

    return 0;
}
#endif
