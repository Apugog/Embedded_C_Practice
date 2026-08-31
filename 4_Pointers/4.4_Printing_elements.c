#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#define SIZE_ARR(x) (sizeof(x)/sizeof(x[0]))

typedef enum{
    PTR_STATUS_SUCCESS = 0,
    PTR_STATUS_NULL_PTR,
    PTR_STATUS_EMPTY
}ptr_status_t;

ptr_status_t print_elements(const uint32_t *arr, size_t len){
    if (arr == NULL){
        return PTR_STATUS_NULL_PTR;
    }

    if (len==0){
        return PTR_STATUS_EMPTY;
    }

    while(len){
        printf("%u ",*arr++);
        len--;
    }

    return PTR_STATUS_SUCCESS;
}

#ifndef TESTING
int main(){

    uint32_t arr[5]= {1, 2, 3, 4, 5}; 
    print_elements(arr,SIZE_ARR(arr));
    return 0;
}
#endif