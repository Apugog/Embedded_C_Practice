#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef enum {
    UART_PARITY_NONE = 0,
    UART_PARITY_EVEN,
    UART_PARITY_ODD
} uart_parity_t;

typedef enum {
    UART_STOP_BITS_1 = 1,
    UART_STOP_BITS_2 = 2
} uart_stop_bits_t;

typedef enum {
    UART_FLOW_CTRL_NONE = 0,
    UART_FLOW_CTRL_RTS_CTS
} uart_flow_ctrl_t;

typedef enum{
    UART_7 = 7,
    UART_8,
    UART_9
} uart_data_bits_t;

typedef enum{
    UART_BUFF_EMPTY,
    UART_BUFF_DATA_READY,
    UART_BUFF_FULL,
    UART_BUFF_OVERRUN
} buff_state_t;

typedef enum{
    UART_OK,
    UART_ERR_NULL_PTR,
    UART_ERR_INVALID_PARAM
} uart_status_t;

typedef struct{
uint32_t baud_rate;
uint8_t parity;
uint8_t stop_bit;
uint8_t flow_control;
uint8_t data_bits;
}uart_config_t;

typedef struct{
    uart_config_t config;
    buff_state_t buff_state; /*EMPTY, DATA_READY, FULL, OVERRUN*/
    uint8_t rx_buff[64];
} uart_device_t;

static const uart_config_t UART_DEFAULT_CONFIG = {
    .baud_rate    = 115200,
    .parity       = UART_PARITY_NONE,
    .stop_bit     = UART_STOP_BITS_1,
    .flow_control = UART_FLOW_CTRL_NONE,
    .data_bits    = UART_8
};

uart_status_t uart_init(uart_device_t *dev, const uart_config_t *config){
    if(dev == NULL){
        return UART_ERR_NULL_PTR;
    }

    if(config == NULL){
        config = &UART_DEFAULT_CONFIG;
    }

    if (config->baud_rate == 0 ||
        config->parity > UART_PARITY_ODD ||
        (config->stop_bit != UART_STOP_BITS_1 && config->stop_bit != UART_STOP_BITS_2) ||
        config->flow_control > UART_FLOW_CTRL_RTS_CTS ||
        config->data_bits < UART_7 || config->data_bits > UART_9) {
        return UART_ERR_INVALID_PARAM;
    }

    dev->config = *config; 
    dev->buff_state = UART_BUFF_EMPTY; 
    memset(dev->rx_buff, 0, sizeof(dev->rx_buff));

    return UART_OK;
}


#ifndef TESTING
int main(){
    uart_config_t uart_1 = {115200, UART_PARITY_NONE, UART_STOP_BITS_1, UART_FLOW_CTRL_NONE, UART_8};
    uart_config_t uart_2 = {.baud_rate=115200, .parity=UART_PARITY_NONE, .stop_bit=UART_STOP_BITS_1, .flow_control=UART_FLOW_CTRL_NONE, .data_bits=UART_8};
    uart_device_t dev_1 = {
        .config = {
            .baud_rate=115200, 
            .parity=UART_PARITY_NONE, 
            .stop_bit=UART_STOP_BITS_1, 
            .flow_control=UART_FLOW_CTRL_NONE, 
            .data_bits=UART_8
            },
            .buff_state = UART_BUFF_EMPTY,
            .rx_buff = {0}
    };

    uart_device_t dev_2 = {0};

    uart_device_t dev1 = {0};
    uart_init(&dev1, &uart_2);

    uart_device_t dev2 = {0};
    uart_init(&dev2, NULL);  // Takes UART_DEFAULT_CONFIG automatically

    uart_device_t dev3 = {0};
    uart_init(&dev3, &(uart_config_t){
        .baud_rate    = 9600,
        .parity       = UART_PARITY_EVEN,
        .stop_bit     = UART_STOP_BITS_1,
        .flow_control = UART_FLOW_CTRL_NONE,
        .data_bits    = UART_8
    });

    return 0;
}
#endif