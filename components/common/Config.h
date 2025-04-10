#pragma once

#if defined(CONFIG_IDF_TARGET_ESP32S3) 

//I2C
#define I2C_SDA     GPIO_NUM_4
#define I2C_SCL     GPIO_NUM_5

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

// camera
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 15
#define SIOD_GPIO_NUM 4
#define SIOC_GPIO_NUM 5

#define Y2_GPIO_NUM 11
#define Y3_GPIO_NUM 9
#define Y4_GPIO_NUM 8
#define Y5_GPIO_NUM 10
#define Y6_GPIO_NUM 12
#define Y7_GPIO_NUM 18
#define Y8_GPIO_NUM 17
#define Y9_GPIO_NUM 16

#define VSYNC_GPIO_NUM 6
#define HREF_GPIO_NUM 7
#define PCLK_GPIO_NUM 13

#elif defined(CONFIG_IDF_TARGET_ESP32C3) 

#endif