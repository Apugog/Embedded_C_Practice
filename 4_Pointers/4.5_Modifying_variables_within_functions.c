#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))

typedef enum{
    STATUS_SUCCESS = 0,
    STATUS_NULL_PTR,
    STATUS_INVALID_PARAM
}ptr_status_t;

ptr_status_t compute_stats(const uint32_t *arr, size_t len, uint32_t *out_max, uint32_t *out_min, uint32_t *out_avg){
    if(arr == NULL || out_max == NULL || out_min == NULL || out_avg == NULL){
        return STATUS_NULL_PTR;
    }

    if(len == 0){
        return STATUS_INVALID_PARAM;
    }

    uint32_t local_max = arr[0];
    uint32_t local_min = arr[0];
    uint64_t sum = 0;

    for(size_t i = 0; i < len; i++){
        if(arr[i] > local_max){
            local_max = arr[i];
        }

        if(arr[i] < local_min){
            local_min = arr[i];
        }

        sum += arr[i];
    }

    *out_max = local_max;
    *out_min = local_min;
    *out_avg = (uint32_t)(sum / len);

    return STATUS_SUCCESS;
}

#ifndef TESTING
int main()
{
    uint32_t arr[5] = {1, 5, 3, 2, 4};
    uint32_t out_max, out_min, out_avg;
    
    ptr_status_t status = compute_stats(arr, ARRAY_SIZE(arr), &out_max, &out_min, &out_avg);
    if(status == STATUS_SUCCESS){
        printf("Max:%u Min:%u Avg:%u\n", out_max, out_min, out_avg);
    } else {
        printf("Error: compute_stats failed with status %d\n", status);
    }

    return 0;
}
#endif