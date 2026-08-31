#include<stdio.h>
#include<stdint.h>
#include<stddef.h>

typedef enum{
    ARR_NULL=0,
    ARR_ZERO_LEN,
    ARR_SUCCESS
} arr_status_t;

arr_status_t print_arr(const uint32_t* a, size_t len)
{
    if(a==NULL)
        return ARR_NULL;

    if(len==0)
        return ARR_ZERO_LEN;


    while(len)
    {
        printf("%u ",*a++);
        len--;
    }
    printf("\n");
    return ARR_SUCCESS;
}

arr_status_t rev_arr(uint32_t* a, size_t len)
{
    if(a==NULL)
        return ARR_NULL;

    if(len==0)
        return ARR_ZERO_LEN;

    uint32_t* start=a;
    uint32_t* end = a + len - 1;

    while(start<end)
    {
        uint32_t temp = *start;
        *start = *end;
        *end = temp;

        start++;
        end--;
    }

    return ARR_SUCCESS;
}

#ifndef TESTING
int main()
{
    uint32_t a[10] = {1,2,3,4,5,6};

    print_arr(a,4);
    rev_arr(a,4);
    print_arr(a,4);
    return 0;    
}
#endif