#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

extern "C" {
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

    typedef enum {
        UART_7 = 7,
        UART_8,
        UART_9
    } uart_data_bits_t;

    typedef enum {
        UART_BUFF_EMPTY,
        UART_BUFF_DATA_READY,
        UART_BUFF_FULL,
        UART_BUFF_OVERRUN
    } buff_state_t;

    typedef enum {
        UART_OK,
        UART_ERR_NULL_PTR,
        UART_ERR_INVALID_PARAM
    } uart_status_t;

    typedef struct {
        uint32_t baud_rate;
        uint8_t parity;
        uint8_t stop_bit;
        uint8_t flow_control;
        uint8_t data_bits;
    } uart_config_t;

    typedef struct {
        uart_config_t config;
        buff_state_t buff_state;
        uint8_t rx_buff[64];
    } uart_device_t;

    uart_status_t uart_init(uart_device_t *dev, const uart_config_t *config);
}

/* --------------------------------------------------------------------------
 * Core Initialization Tests (uart_init)
 * -------------------------------------------------------------------------- */

TEST(UartInitTest, RejectsNullDevicePointer) {
    uart_config_t cfg = {
        .baud_rate    = 115200,
        .parity       = UART_PARITY_NONE,
        .stop_bit     = UART_STOP_BITS_1,
        .flow_control = UART_FLOW_CTRL_NONE,
        .data_bits    = UART_8
    };

    EXPECT_EQ(uart_init(NULL, &cfg), UART_ERR_NULL_PTR);
    EXPECT_EQ(uart_init(NULL, NULL), UART_ERR_NULL_PTR);
}

TEST(UartInitTest, NullConfigAppliesHardwareDefaults) {
    uart_device_t dev;
    memset(&dev, 0xA5, sizeof(dev)); // Poison memory with pattern

    uart_status_t status = uart_init(&dev, NULL);

    EXPECT_EQ(status, UART_OK);
    EXPECT_EQ(dev.config.baud_rate, 115200u);
    EXPECT_EQ(dev.config.parity, UART_PARITY_NONE);
    EXPECT_EQ(dev.config.stop_bit, UART_STOP_BITS_1);
    EXPECT_EQ(dev.config.flow_control, UART_FLOW_CTRL_NONE);
    EXPECT_EQ(dev.config.data_bits, UART_8);
    EXPECT_EQ(dev.buff_state, UART_BUFF_EMPTY);

    for (size_t i = 0; i < sizeof(dev.rx_buff); ++i) {
        EXPECT_EQ(dev.rx_buff[i], 0u);
    }
}

TEST(UartInitTest, AppliesCustomConfiguration) {
    uart_device_t dev;
    uart_config_t custom_cfg = {
        .baud_rate    = 9600,
        .parity       = UART_PARITY_EVEN,
        .stop_bit     = UART_STOP_BITS_2,
        .flow_control = UART_FLOW_CTRL_RTS_CTS,
        .data_bits    = UART_7
    };

    uart_status_t status = uart_init(&dev, &custom_cfg);

    EXPECT_EQ(status, UART_OK);
    EXPECT_EQ(dev.config.baud_rate, 9600u);
    EXPECT_EQ(dev.config.parity, UART_PARITY_EVEN);
    EXPECT_EQ(dev.config.stop_bit, UART_STOP_BITS_2);
    EXPECT_EQ(dev.config.flow_control, UART_FLOW_CTRL_RTS_CTS);
    EXPECT_EQ(dev.config.data_bits, UART_7);
    EXPECT_EQ(dev.buff_state, UART_BUFF_EMPTY);
}

TEST(UartInitTest, RejectsZeroBaudRate) {
    uart_device_t dev;
    uart_config_t cfg = {
        .baud_rate    = 0,
        .parity       = UART_PARITY_NONE,
        .stop_bit     = UART_STOP_BITS_1,
        .flow_control = UART_FLOW_CTRL_NONE,
        .data_bits    = UART_8
    };

    EXPECT_EQ(uart_init(&dev, &cfg), UART_ERR_INVALID_PARAM);
}

TEST(UartInitTest, RejectsInvalidParity) {
    uart_device_t dev;
    uart_config_t cfg = {
        .baud_rate    = 115200,
        .parity       = (uint8_t)(UART_PARITY_ODD + 1),
        .stop_bit     = UART_STOP_BITS_1,
        .flow_control = UART_FLOW_CTRL_NONE,
        .data_bits    = UART_8
    };

    EXPECT_EQ(uart_init(&dev, &cfg), UART_ERR_INVALID_PARAM);
}

TEST(UartInitTest, RejectsInvalidStopBits) {
    uart_device_t dev;
    uart_config_t cfg_zero = {
        .baud_rate    = 115200,
        .parity       = UART_PARITY_NONE,
        .stop_bit     = 0,
        .flow_control = UART_FLOW_CTRL_NONE,
        .data_bits    = UART_8
    };
    uart_config_t cfg_high = {
        .baud_rate    = 115200,
        .parity       = UART_PARITY_NONE,
        .stop_bit     = 3,
        .flow_control = UART_FLOW_CTRL_NONE,
        .data_bits    = UART_8
    };

    EXPECT_EQ(uart_init(&dev, &cfg_zero), UART_ERR_INVALID_PARAM);
    EXPECT_EQ(uart_init(&dev, &cfg_high), UART_ERR_INVALID_PARAM);
}

TEST(UartInitTest, RejectsInvalidFlowControl) {
    uart_device_t dev;
    uart_config_t cfg = {
        .baud_rate    = 115200,
        .parity       = UART_PARITY_NONE,
        .stop_bit     = UART_STOP_BITS_1,
        .flow_control = (uint8_t)(UART_FLOW_CTRL_RTS_CTS + 1),
        .data_bits    = UART_8
    };

    EXPECT_EQ(uart_init(&dev, &cfg), UART_ERR_INVALID_PARAM);
}

TEST(UartInitTest, RejectsInvalidDataBits) {
    uart_device_t dev;
    uart_config_t cfg_low = {
        .baud_rate    = 115200,
        .parity       = UART_PARITY_NONE,
        .stop_bit     = UART_STOP_BITS_1,
        .flow_control = UART_FLOW_CTRL_NONE,
        .data_bits    = 6
    };
    uart_config_t cfg_high = {
        .baud_rate    = 115200,
        .parity       = UART_PARITY_NONE,
        .stop_bit     = UART_STOP_BITS_1,
        .flow_control = UART_FLOW_CTRL_NONE,
        .data_bits    = 10
    };

    EXPECT_EQ(uart_init(&dev, &cfg_low), UART_ERR_INVALID_PARAM);
    EXPECT_EQ(uart_init(&dev, &cfg_high), UART_ERR_INVALID_PARAM);
}

TEST(UartInitTest, ClearsBufferAndStateOnInitialization) {
    uart_device_t dev;
    memset(&dev, 0xFF, sizeof(dev)); // Fill with garbage bytes

    uart_config_t cfg = {
        .baud_rate    = 57600,
        .parity       = UART_PARITY_NONE,
        .stop_bit     = UART_STOP_BITS_1,
        .flow_control = UART_FLOW_CTRL_NONE,
        .data_bits    = UART_8
    };

    EXPECT_EQ(uart_init(&dev, &cfg), UART_OK);
    EXPECT_EQ(dev.buff_state, UART_BUFF_EMPTY);
    for (size_t i = 0; i < sizeof(dev.rx_buff); ++i) {
        EXPECT_EQ(dev.rx_buff[i], 0u);
    }
}
