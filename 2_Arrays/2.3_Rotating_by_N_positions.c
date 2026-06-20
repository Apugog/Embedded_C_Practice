#include <stdio.h>
#include <stdint.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

typedef enum{
    ARRAY_SUCCESS = 0,
    ARRAY_ERR_NULL,
    ARRAY_ERR_EMPTY
} array_status_t;

typedef enum{
    ROTATION_LEFT = 0,
    ROTATION_RIGHT
} rotation_dir_t;

static void reverse_array(int32_t* arr, size_t l, size_t r)
{
    while(l<r)
    {
        int32_t temp = arr[l];
        arr[l] = arr[r];
        arr[r] = temp;
        l++;
        r--;
    }
}

static void print_arr(int32_t* arr, size_t len)
{
    for(size_t i = 0; i< len; i++)
        printf("%d ",arr[i]);
    printf("\n");
}

array_status_t rotate_array(int32_t* arr, size_t len, size_t k, rotation_dir_t dir)
{
    if(arr == NULL)
        return ARRAY_ERR_NULL;
    
    if(len==0)
        return ARRAY_ERR_EMPTY;

    k = k % len;

    if(k==0)
        return ARRAY_SUCCESS;

    if(dir==ROTATION_RIGHT)
    {
        reverse_array(arr, 0,len-1);
        reverse_array(arr, 0, k-1);
        reverse_array(arr, k, len-1);
    }
    else if(dir==ROTATION_LEFT)
    {                
        reverse_array(arr, 0, k-1);
        reverse_array(arr, k, len-1);
        reverse_array(arr, 0,len-1);
    }

    return ARRAY_SUCCESS;
}

#ifndef TESTING
int main()
{
    int32_t arr[] = {1,2,3,4,5,6,7,8};
    print_arr(arr, ARRAY_SIZE(arr));
    rotate_array(arr, ARRAY_SIZE(arr), 3, ROTATION_LEFT);
    print_arr(arr, ARRAY_SIZE(arr));
    return 0;
}
#endif