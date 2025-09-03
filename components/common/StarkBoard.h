#pragma once
#include "driver/gpio.h"

//I2C
#define I2C_SDA     GPIO_NUM_1
#define I2C_SCL     GPIO_NUM_2

// Battery
#define BATTERY_ADC                 (GPIO_NUM_10)

// 定义 INMP441 连接的 ESP32S3 引脚
// INMP441 config
// #define I2S_IN_PORT I2S_NUM_0
#define I2S_MIC_BCLK        48//40 // SCK
#define I2S_MIC_LRC         10//42 // WS
#define I2S_MIC_DIN         45//41 // SD
#define MIC_SAMPLE_RATE         16000
#define MIC_BIT_WIDTH I2S_DATA_BIT_WIDTH_32BIT

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

// camera OV2640
// #define PWDN_GPIO_NUM -1
// #define RESET_GPIO_NUM -1
// #define XCLK_GPIO_NUM 8
// #define SIOD_GPIO_NUM 1
// #define SIOC_GPIO_NUM 2

// #define Y2_GPIO_NUM 7
// #define Y3_GPIO_NUM 5
// #define Y4_GPIO_NUM 4
// #define Y5_GPIO_NUM 6
// #define Y6_GPIO_NUM 15
// #define Y7_GPIO_NUM 17
// #define Y8_GPIO_NUM 18
// #define Y9_GPIO_NUM 3

// #define VSYNC_GPIO_NUM 9
// #define HREF_GPIO_NUM 46
// #define PCLK_GPIO_NUM 16

// GC0308
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 3
#define SIOD_GPIO_NUM 1
#define SIOC_GPIO_NUM 2

#define Y2_GPIO_NUM 7
#define Y3_GPIO_NUM 4
#define Y4_GPIO_NUM 5
#define Y5_GPIO_NUM 6
#define Y6_GPIO_NUM 15
#define Y7_GPIO_NUM 17
#define Y8_GPIO_NUM 18
#define Y9_GPIO_NUM 8

#define VSYNC_GPIO_NUM 9
#define HREF_GPIO_NUM 46
#define PCLK_GPIO_NUM 16



/***********************************************************/
/****************    LCD显示屏 ↓   *************************/
#define TFT_PIXEL_CLOCK_HZ     (80 * 1000 * 1000)
#define TFT_SPI_NUM            (SPI3_HOST)
#define TFT_CMD_BITS               (8)
#define TFT_PARAM_BITS             (8)
#define TFT_BITS_PER_PIXEL     (16)
#define TFT_LEDC_CH                 LEDC_CHANNEL_0
#define TFT_SPI_MODE                2
#define TFT_INVERT_COLOR            false
#define TFT_MIRROR_X false
#define TFT_MIRROR_Y false
#define TFT_SWAP_XY false

#define TFT_LCD_H_RES              (240)
#define TFT_LCD_V_RES              (240)

// #define DRIVER_GC9A01
#define TFT_DRIVER_ST7789
#define USE_LVGL                    false
#define TFT_HAS_TOUCH               false

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

#define TFT_LCD_SPI_MOSI      (GPIO_NUM_41)
#define TFT_LCD_SPI_CLK       (GPIO_NUM_40)
#define TFT_LCD_SPI_CS        (GPIO_NUM_39)
#define TFT_LCD_DC            (GPIO_NUM_38)
#define TFT_LCD_RST           (GPIO_NUM_NC)
#define TFT_LCD_BACKLIGHT     (GPIO_NUM_42)  
#define TFT_BACKLIGHT_OUTPUT_INVERT false