#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))

typedef enum{
    STATUS_SUCCESS,
    STATUS_NULL_PTR,
    STATUS_INVALID_PARAM
} status_t;

typedef struct{
    uint32_t alarm_count;
    uint8_t max_val;
}monitor_context_t;

typedef void (*threshold_cb)(uint8_t val, size_t index, void *user_data);

void pr_over_temp(uint8_t val, size_t index, void *user_data){
    printf("\nover temp detected: %u at index %zu", val, index);

    if(user_data != NULL){
      monitor_context_t *ctx = (monitor_context_t *)user_data;
      ctx->alarm_count++;
      if (val> ctx->max_val){
        ctx->max_val = val;
      }
    }
}


status_t sensor_mon(const uint8_t *arr, size_t len, uint8_t threshold, threshold_cb call_back, void *user_data){
    if(arr == NULL || call_back == NULL){
        return STATUS_NULL_PTR;
    }

    if(len == 0){
        return STATUS_INVALID_PARAM;
    }

    for(size_t i = 0; i < len; i++){
        if(arr[i] > threshold){
            call_back(arr[i], i, user_data);
        }
    }

    return STATUS_SUCCESS;
}

#ifndef TESTING
int main(){
    uint8_t sensor_val[6] = {1, 3, 7, 8, 2, 5};
    monitor_context_t ctx = {0};
    
    sensor_mon(sensor_val, ARRAY_SIZE(sensor_val), 3, pr_over_temp, &ctx);

    printf("\n\n--- Summary ---");
    printf("\nTotal Alarms: %u", ctx.alarm_count);
    printf("\nMax Temp seen: %u\n", ctx.max_val);
    
    return 0;
}
#endif