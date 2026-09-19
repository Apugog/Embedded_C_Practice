#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SENSORS 8

typedef enum{
    SENSOR_OK,
    SENSOR_DISCONNECTED,
    SENSOR_OUT_OF_BOUNDS
}sensor_status_t;

typedef struct{
    uint8_t sensor_id;
    char name[15];
    int32_t raw_val;
    sensor_status_t status;
} sensor_t;

typedef struct{
    sensor_t sensors[MAX_SENSORS];
    size_t count;
} sensor_registry_t;

typedef enum{
    REG_OK,
    REG_ERR_NULL_PTR,
    REG_ERR_FULL,
    REG_ERR_DUPLICATE_ID,
    REG_ERR_NOT_FOUND,
    REG_ERR_INVALID_PARAM,
    REG_ERR_EMPTY
}registry_status_t;

registry_status_t registry_init(sensor_registry_t *reg){
    if(reg==NULL){
        return REG_ERR_NULL_PTR;
    }
    memset(reg->sensors,0,sizeof(reg->sensors));
    reg->count = 0;
    return REG_OK;
}

registry_status_t registry_add(sensor_registry_t *reg,const sensor_t *sensor){
    if(sensor==NULL || reg==NULL){
        return REG_ERR_NULL_PTR;
    }

    if(reg->count >= MAX_SENSORS){
        return REG_ERR_FULL;
    }

    if(sensor->sensor_id == 0){
        return REG_ERR_INVALID_PARAM;
    }

    for(size_t i=0; i<reg->count;i++){
        if(reg->sensors[i].sensor_id == sensor->sensor_id){
            return REG_ERR_DUPLICATE_ID;
        }
    }

    // save data (unlike array struct are first class data types can be copied directly like int)
    reg->sensors[reg->count] = *sensor;
    // Guarantee null-termination defensively:
    reg->sensors[reg->count].name[sizeof(reg->sensors[0].name)-1] = '\0';
    // increment count
    reg->count++;
    
    return REG_OK;
}

registry_status_t registry_find_by_id(sensor_registry_t *reg, uint8_t id, sensor_t **out_sensor){
    if(reg==NULL || out_sensor==NULL){
        return REG_ERR_NULL_PTR;
    }

    *out_sensor = NULL;

    if(reg->count == 0){
        return REG_ERR_EMPTY;
    }

    for(size_t i=0; i<reg->count; i++){
        if(reg->sensors[i].sensor_id == id){
            *out_sensor=&(reg->sensors[i]);
            return REG_OK;
        }
    }

    return REG_ERR_NOT_FOUND;
}

registry_status_t registry_update_reading(sensor_registry_t *reg, uint8_t id, int32_t raw_val, sensor_status_t status){
    if(reg==NULL){
        return REG_ERR_NULL_PTR;
    }

    if(reg->count==0){
        return REG_ERR_EMPTY;
    }

    if(status>SENSOR_OUT_OF_BOUNDS){
        return REG_ERR_INVALID_PARAM;
    }

    sensor_t *temp;

    registry_status_t res = registry_find_by_id(reg, id, &temp);
    if(res != REG_OK){
        return REG_ERR_NOT_FOUND;
    }

    temp->raw_val = raw_val;
    temp->status = status;

    return REG_OK;
}

registry_status_t registry_get_max_reading(const sensor_registry_t *reg, int32_t *out_max_val){
    if(reg == NULL || out_max_val == NULL){
        return REG_ERR_NULL_PTR;
    }

    if(reg->count==0){
        return REG_ERR_EMPTY;
    }

    bool found_valid = 0;
    int32_t max_val = 0;

    for(size_t i=0 ; i < reg->count; i++){
        if(reg->sensors[i].status == SENSOR_OK){
            // once found_valid is set it turns off entry using "!" operator
            if(!found_valid || reg->sensors[i].raw_val > max_val){
                max_val = reg->sensors[i].raw_val;
                found_valid = 1;
            }
        }
    }

    if (!found_valid) {
        return REG_ERR_NOT_FOUND; // No healthy sensors were found
    }

    *out_max_val = max_val;

    return REG_OK;
}

size_t registry_count_by_state(const sensor_registry_t *reg, sensor_status_t status){
    if(reg==NULL){
        return 0;
    }

    if(status> SENSOR_OUT_OF_BOUNDS){
        return 0;
    }

    size_t count = 0;

    for(size_t i=0; i < reg->count; i++){
        if(reg->sensors[i].status==status){
            count++;
        }
    }

    return count;
}

#ifndef TESTING
int main(){
    sensor_registry_t reg;
    registry_init(&reg);

    return 0;
}
#endif