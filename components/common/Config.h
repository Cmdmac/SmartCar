#pragma once
#include "driver/gpio.h"
#if defined(CONFIG_IDF_TARGET_ESP32S3) 

//I2C
#define I2C_SDA     GPIO_NUM_12
#define I2C_SCL     GPIO_NUM_13

// 定义 INMP441 连接的 ESP32S3 引脚
// INMP441 config
// #define I2S_IN_PORT I2S_NUM_0
#define I2S_IN_BCLK 5//40 // SCK
#define I2S_IN_LRC 4//42 // WS
#define I2S_IN_DIN 6//41 // SD

#define I2S_SPK_DOUT      12
#define I2S_SPK_BCLK      11
#define I2S_SPK_LRC       10

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


/***********************************************************/
/****************    LCD显示屏 ↓   *************************/
#define TFT_LCD_PIXEL_CLOCK_HZ     (80 * 1000 * 1000)
#define TFT_LCD_SPI_NUM            (SPI3_HOST)
#define LCD_CMD_BITS               (8)
#define LCD_PARAM_BITS             (8)
#define TFT_LCD_BITS_PER_PIXEL     (16)
#define LCD_LEDC_CH                 LEDC_CHANNEL_0
#define TFT_INVERT_COLOR                false

#define TFT_LCD_H_RES              (128)
#define TFT_LCD_V_RES              (128)

#define DRIVER_GC9A01
// #define DRIVER_ST7789
#define USE_LVGL                    false
#define TFT_HAS_TOUCH               false

#define BSP_I2C_NUM           I2C_NUM_0             // I2C外设
#define BSP_I2C_FREQ_HZ       100000         // 100kHz

// #define BSP_LCD_SPI_MOSI      (GPIO_NUM_15)
// #define BSP_LCD_SPI_CLK       (GPIO_NUM_16)
// #define BSP_LCD_SPI_CS        (GPIO_NUM_5)
// #define BSP_LCD_DC            (GPIO_NUM_6)
// #define BSP_LCD_RST           (GPIO_NUM_7)
// #define BSP_LCD_BACKLIGHT     (GPIO_NUM_4)  

// #define BSP_LCD_SPI_MOSI      (GPIO_NUM_38)
// #define BSP_LCD_SPI_CLK       (GPIO_NUM_39)
// #define BSP_LCD_SPI_CS        (GPIO_NUM_40)
// #define BSP_LCD_DC            (GPIO_NUM_41)
// #define BSP_LCD_RST           (GPIO_NUM_NC)
// #define BSP_LCD_BACKLIGHT     (GPIO_NUM_12)  
// #define BSP_LCD_SPI_MOSI      (GPIO_NUM_41)
// #define BSP_LCD_SPI_CLK       (GPIO_NUM_40)
// #define BSP_LCD_SPI_CS        (GPIO_NUM_39)
// #define BSP_LCD_DC            (GPIO_NUM_38)
// #define BSP_LCD_RST           (GPIO_NUM_NC)
// #define BSP_LCD_BACKLIGHT     (GPIO_NUM_42)  

#define TFT_LCD_SPI_MOSI      (GPIO_NUM_3)
#define TFT_LCD_SPI_CLK       (GPIO_NUM_8)
#define TFT_LCD_SPI_CS        (GPIO_NUM_18)
#define TFT_LCD_DC            (GPIO_NUM_46)
#define TFT_LCD_RST           (GPIO_NUM_NC)
#define TFT_LCD_BACKLIGHT     (GPIO_NUM_NC)  

#elif defined(CONFIG_IDF_TARGET_ESP32C3) 

#endif