#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
    GEN_STATUS_SUCCESS = 0,
    GEN_STATUS_NULL_PTR,
    GEN_STATUS_EMPTY,
    GEN_STATUS_INVALID_SIZE
} gen_status_t;

typedef void (*elem_print_fn)(const void *elem);

void print_uint32(const void *elem){
    printf("%u", *(const uint32_t*)elem);
}

void print_float(const void *elem){
    printf("%.2f", *(const float *)elem);
}

typedef struct {
    uint8_t id;
    int16_t temp;
} sensor_t;

void print_sensor(const void *elem){
    const sensor_t *s = (const sensor_t *)elem;
    printf("[ID: %u, Temp: %d C]", s->id, s->temp);
}

gen_status_t print_generic_array(const void *base,
                                size_t num_elem,
                                size_t elem_size,
                                elem_print_fn print_cb){                
    if (base == NULL || print_cb == NULL) {
        return GEN_STATUS_NULL_PTR;
    }

    if (num_elem == 0) {
        return GEN_STATUS_EMPTY;
    }

    if (elem_size == 0) {
        return GEN_STATUS_INVALID_SIZE;
    }

    for (size_t i = 0; i < num_elem; i++) {
        const uint8_t *address = (const uint8_t *)base + (i * elem_size);
        print_cb(address);
        printf(" ");
    }
    printf("\n");

    return GEN_STATUS_SUCCESS;
}

#ifndef TESTING
int main(void) {
    // 1. Array of uint32_t
    uint32_t int_data[] = {10, 20, 30, 40, 50};
    size_t int_len = sizeof(int_data) / sizeof(int_data[0]);
    printf("uint32_t array: ");
    print_generic_array(int_data, int_len, sizeof(uint32_t), print_uint32);

    // 2. Array of float
    float float_data[] = {3.14f, 2.71f, 1.41f, 0.57f};
    size_t float_len = sizeof(float_data) / sizeof(float_data[0]);
    printf("float array:    ");
    print_generic_array(float_data, float_len, sizeof(float), print_float);

    // 3. Array of sensor_t structs
    sensor_t sensor_data[] = {
        {.id = 1, .temp = 25},
        {.id = 2, .temp = -5},
        {.id = 3, .temp = 85}
    };
    size_t sensor_len = sizeof(sensor_data) / sizeof(sensor_data[0]);
    printf("sensor_t array: ");
    print_generic_array(sensor_data, sensor_len, sizeof(sensor_t), print_sensor);

    return 0;
}
#endif