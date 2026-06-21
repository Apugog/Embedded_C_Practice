#include <stdio.h>
#include <stdint.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0])) 

typedef enum{
    ARRAY_SUCCESS = 0,
    ARRAY_ERR_NULL,
    ARRAY_ERR_EMPTY
}array_status_t;

array_status_t remove_duplicates(int32_t* arr, size_t len, size_t* new_len)
{
    if(arr == NULL|| new_len == NULL)
        return ARRAY_ERR_NULL;

    if(len == 0)
        return ARRAY_ERR_EMPTY;

    size_t l=0;
    
    for(size_t r =0; r<len;r++)
    {
        if(arr[r]!=arr[l])
        {
            arr[++l] = arr[r];
        }
    }
    *new_len = l+1;

    return ARRAY_SUCCESS;
}

static void print_arr(int32_t* arr, size_t len)
{
    for(size_t i = 0; i< len; i++)
        printf("%d ",arr[i]);
    printf("\n");
}

#ifndef TESTING
int main()
{
    int32_t arr[] = {1, 1, 2, 3, 4, 4, 6};
    print_arr(arr, ARRAY_SIZE(arr));
    size_t new_len;
    remove_duplicates(arr, ARRAY_SIZE(arr),&new_len);
    print_arr(arr, new_len);
}
#endif