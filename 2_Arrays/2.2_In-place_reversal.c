#include <stdio.h>
#include <stdint.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

typedef enum{
    ARRAY_SUCCESS = 0,
    ARRAY_ERR_NULL,
    ARRAY_ERR_EMPTY
} array_status_t;

array_status_t reverse_array(int32_t* arr, size_t len)
{
    if(arr ==NULL)
        return ARRAY_ERR_NULL;

    if(len==0)
        return ARRAY_ERR_EMPTY;
    
    size_t l,r;
    l = 0;
    r = len-1;

    while(l<r)
    {
        int32_t temp = arr[l];
        arr[l] = arr[r];
        arr[r] = temp;
        l++;
        r--;
    }

    return ARRAY_SUCCESS;
}

void print_arr(int32_t* arr, size_t len)
{
    for(size_t i = 0; i< len; i++)
        printf("%d ",arr[i]);
    printf("\n");
}

#ifndef TESTING
int main()
{
    int32_t arr[] = {1,2,3,4,5,6,7,8};
    print_arr(arr, ARRAY_SIZE(arr));
    reverse_array(arr, ARRAY_SIZE(arr));
    print_arr(arr, ARRAY_SIZE(arr));
    return 0;
}
#endif