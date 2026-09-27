#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

typedef struct{
    uint8_t  sensor_id;
    uint32_t timestamp;
    uint8_t  flag;
    uint32_t reading;
    uint16_t checksum;
}sensor_payload_t;

typedef struct{
    uint32_t timestamp;
    uint32_t reading;
    uint16_t checksum;
    uint8_t  sensor_id;
    uint8_t  flag;
}sensor_payload2_t;

#pragma pack(push, 1)
typedef struct {
    uint8_t  sensor_id;
    uint32_t timestamp;
    uint8_t  flag;
    uint32_t reading;
    uint16_t checksum;
} sensor_payload_packed_t;
#pragma pack(pop)

#include <stdbool.h>
#include <string.h>

/* Safe deserialization: unpack wire format into naturally aligned struct */
bool unpack_sensor_payload(const sensor_payload_packed_t *src, sensor_payload2_t *dest) {
    if(src == NULL || dest == NULL){
        return false;
    }

    memset(dest,0,sizeof(sensor_payload2_t));
    /* TODO: Implement defensive checks and safe field extraction */
    dest->sensor_id = src->sensor_id;
    dest->flag = src->flag;
    //&(dest->reading) is equal to &dest->reading as -> has a greater precedence
    memcpy(&dest->reading,&src->reading,sizeof(dest->reading));
    memcpy(&dest->checksum,&src->checksum,sizeof(dest->checksum));
    memcpy(&dest->timestamp,&src->timestamp,sizeof(dest->timestamp));

    return true;
}

/* Safe serialization: pack naturally aligned struct into compact wire format */
bool pack_sensor_payload(const sensor_payload2_t *src, sensor_payload_packed_t *dest) {
    if (src==NULL || dest==NULL){
        return false;
    }
    /* TODO: Implement defensive checks and safe packing */
    dest->sensor_id = src->sensor_id;
    dest->flag = src->flag;
    
    memcpy(&dest->reading, &src->reading, sizeof(dest->reading));
    memcpy(&dest->checksum, &src->checksum, sizeof(dest->checksum));
    memcpy(&dest->timestamp, &src->timestamp, sizeof(dest->timestamp));
    
    return true;
}

#ifndef TESTING
int main(void) {
    printf("=== 1. Unoptimized (Size: %zu, Padding: %zu bytes) ===\n",
           sizeof(sensor_payload_t), sizeof(sensor_payload_t) - 12);
    printf("sensor_id: %zu | timestamp: %zu | flag: %zu | reading: %zu | checksum: %zu\n\n",
           offsetof(sensor_payload_t, sensor_id),
           offsetof(sensor_payload_t, timestamp),
           offsetof(sensor_payload_t, flag),
           offsetof(sensor_payload_t, reading),
           offsetof(sensor_payload_t, checksum));

    printf("=== 2. Reordered (Size: %zu, Padding: %zu bytes) ===\n",
           sizeof(sensor_payload2_t), sizeof(sensor_payload2_t) - 12);
    printf("timestamp: %zu | reading: %zu | checksum: %zu | sensor_id: %zu | flag: %zu\n\n",
           offsetof(sensor_payload2_t, timestamp),
           offsetof(sensor_payload2_t, reading),
           offsetof(sensor_payload2_t, checksum),
           offsetof(sensor_payload2_t, sensor_id),
           offsetof(sensor_payload2_t, flag));

    printf("=== 3. Packed (Size: %zu, Padding: %zu bytes) ===\n",
           sizeof(sensor_payload_packed_t), sizeof(sensor_payload_packed_t) - 12);
    printf("sensor_id: %zu | timestamp: %zu | flag: %zu | reading: %zu | checksum: %zu\n",
           offsetof(sensor_payload_packed_t, sensor_id),
           offsetof(sensor_payload_packed_t, timestamp),
           offsetof(sensor_payload_packed_t, flag),
           offsetof(sensor_payload_packed_t, reading),
           offsetof(sensor_payload_packed_t, checksum));

    return 0;
}
#endif