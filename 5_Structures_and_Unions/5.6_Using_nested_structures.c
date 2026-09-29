#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct{
  int16_t offset;
  int16_t scale;
  uint8_t is_valid;
} calib_t;

typedef struct{
    uint32_t id;
    calib_t calib;
    int16_t last_raw;
    uint8_t is_enabled;
} sensor_t;

typedef enum{
    STATUS_OK,
    STATUS_NULL_PTR,
    STATUS_INVALID_PARAM,
    STATUS_UNINITIALIZED
} sensor_status_t;

sensor_status_t calib_apply(const calib_t *cal, int16_t raw, int32_t *out){
    if(cal == NULL || out == NULL){
        return STATUS_NULL_PTR;
    }

    if(cal->is_valid == 0){
        return STATUS_UNINITIALIZED;
    }

    *out = ((int32_t)raw * (int32_t)cal->scale) + (int32_t)cal->offset;

    return STATUS_OK;
}

sensor_status_t sensor_init(sensor_t *dev, uint32_t id, const calib_t *cal){
    if(dev == NULL){
        return STATUS_NULL_PTR;
    }

    if(id == 0){
        return STATUS_INVALID_PARAM;
    }

    memset(dev, 0, sizeof(*dev));

    dev->id = id;
    dev->is_enabled = 1;
    dev->last_raw = 0;

    if(cal != NULL){
        dev->calib = *cal;
    }

    return STATUS_OK;
}

sensor_status_t sensor_read(sensor_t *dev, int16_t raw_val, int32_t *calibrated_out){
    if(dev == NULL || calibrated_out == NULL){
        return STATUS_NULL_PTR;
    }

    if(dev->is_enabled == 0){
        return STATUS_INVALID_PARAM;
    }

    dev->last_raw = raw_val;

    return calib_apply(&dev->calib, raw_val, calibrated_out);
}

#ifndef TESTING
int main(){
    sensor_t temp_sensor;
    calib_t cal = {.offset = -50, .scale = 2, .is_valid = 1};

    sensor_status_t status = sensor_init(&temp_sensor, 0x1001, &cal);
    printf("Init status: %d (0 = OK)\n", status);

    int32_t calibrated_val = 0;
    int16_t raw_adc = 300;
    status = sensor_read(&temp_sensor, raw_adc, &calibrated_val);
    printf("Raw: %d -> Calibrated: %ld (Expected: %d)\n", 
            raw_adc, (long)calibrated_val, (300 * 2) - 50);

    return 0;
}
#endif