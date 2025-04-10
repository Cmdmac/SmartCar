#pragma once

#if defined(CONFIG_IDF_TARGET_ESP32S3) 

// 定义 INMP441 连接的 ESP32S3 引脚
// INMP441 config
// #define I2S_IN_PORT I2S_NUM_0
#define I2S_IN_BCLK 5//40 // SCK
#define I2S_IN_LRC 4//42 // WS
#define I2S_IN_DIN 6//41 // SD

#define I2S_DOUT      42
#define I2S_BCLK      2
#define I2S_LRC       1

// ir
#define IR_TX_GPIO_NUM                  GPIO_NUM_39
#define IR_RX_GPIO_NUM                  GPIO_NUM_10

//sdcard
#define SPI_CS 46
#define SPI_MOSI 3
#define SPI_CLK 20
#define SPI_MISO 19


#elif defined(CONFIG_IDF_TARGET_ESP32C3) 

#endif