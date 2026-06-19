#include <stdio.h>
#include <stdint.h>

typedef enum {                                                                                                                                  
    ARRAY_SUCCESS = 0,                                                                                                                          
    ARRAY_ERR_NULL,                                                                                                                             
    ARRAY_ERR_EMPTY,                                                                                                                            
    ARRAY_ERR_NO_SECOND_LARGEST                                                                                                                 
} array_status_t;                                                                                                                               
                         

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

array_status_t largest(const int32_t* arr, size_t len, int32_t* result)
{
    if(arr == NULL)
        return ARRAY_ERR_NULL;

    if(len == 0)
        return ARRAY_ERR_EMPTY;
        
    *result = INT32_MIN;

    for(int32_t i =0; i< len;i++)
    {
        if(arr[i]>*result)
            *result = arr[i];
    }

    return ARRAY_SUCCESS;
}

array_status_t smallest(const int32_t* arr, size_t len, int32_t* result)
{
    if(arr == NULL)
        return ARRAY_ERR_NULL;

    if(len == 0)
        return ARRAY_ERR_EMPTY;
        
    *result = INT32_MAX;

    for(int32_t i =0; i< len;i++)
    {
        if(arr[i]<*result)
            *result = arr[i];
    }

    return ARRAY_SUCCESS;
}

array_status_t second_largest(const int32_t* arr, size_t len, int32_t* result)
{
    if(arr == NULL)
        return ARRAY_ERR_NULL;

    if (len < 2)                                                                                                                       
        return ARRAY_ERR_NO_SECOND_LARGEST;                                                                                                         
        
    int32_t max = arr[0];
    *result = INT32_MIN;
    int has_second = 0;

    for(size_t i =1; i< len;i++)
    {
        if(arr[i]>max)
        {
            *result = max;
            max = arr[i];
            has_second = 1;
        }
        else if(arr[i]<max)
        {
            if(!has_second || arr[i]>*result)
            {
                *result = arr[i];
                has_second = 1;
            }
        }
    }

    if (!has_second) {                                                                                                                              
        return ARRAY_ERR_NO_SECOND_LARGEST;                                                                                                         
    }

    return ARRAY_SUCCESS;
}

#ifndef TESTING
void main()
{
    const int32_t arr[] = {12, 35, 1, 10, 34, 1};
    int32_t max;
    largest(arr,ARRAY_SIZE(arr),&max);
    printf("%d\n", max);
    smallest(arr,ARRAY_SIZE(arr),&max);
    printf("%d\n", max);
    second_largest(arr,ARRAY_SIZE(arr),&max);
    printf("%d\n", max);

}
#endif