#pragma once
#include <string.h>
#include "math.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_types.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lvgl_port.h"

#include "esp_lcd_touch_cst816s.h"


/***********************************************************/
/****************    LCD显示屏 ↓   *************************/
#define BSP_LCD_PIXEL_CLOCK_HZ     (80 * 1000 * 1000)
#define BSP_LCD_SPI_NUM            (SPI3_HOST)
#define LCD_CMD_BITS               (8)
#define LCD_PARAM_BITS             (8)
#define BSP_LCD_BITS_PER_PIXEL     (16)
#define LCD_LEDC_CH          LEDC_CHANNEL_0

#define BSP_LCD_H_RES              (240)
#define BSP_LCD_V_RES              (240)

#define BSP_LCD_SPI_MOSI      (GPIO_NUM_15)
#define BSP_LCD_SPI_CLK       (GPIO_NUM_16)
#define BSP_LCD_SPI_CS        (GPIO_NUM_5)
#define BSP_LCD_DC            (GPIO_NUM_6)
#define BSP_LCD_RST           (GPIO_NUM_7)
#define BSP_LCD_BACKLIGHT     (GPIO_NUM_4)  

// #define DRIVER_GC9A01
#define DRIVER_ST7789

#define BSP_LCD_DRAW_BUF_HEIGHT    (20)

// #define BSP_I2C_NUM           (0)            // I2C外设


class TFT_SPI {
    public:
        void setup();
        void setBackgroundColor(uint16_t color);
        void drawPicture(int x_start, int y_start, int x_end, int y_end, const unsigned char *gImage);

    private:
        bool init();

        lv_indev_t *initTouch(lv_disp_t *disp);
        esp_err_t initTouchDriver(esp_lcd_touch_handle_t *ret_touch);

        esp_err_t init_brightness();
        esp_err_t setBrightness(int brightness);
        esp_err_t turnOnBacklight();
        esp_err_t turnOffBacklight();
        spi_bus_config_t createBusConfig();
        esp_lcd_panel_io_spi_config_t createIoConfig();

        esp_lcd_panel_handle_t panel_handle = NULL;
        esp_lcd_touch_handle_t tp;   // 触摸屏句柄
        
};
// esp_err_t bsp_display_brightness_init(void);
// esp_err_t bsp_display_brightness_set(int brightness_percent);
// esp_err_t bsp_display_backlight_off(void);
// esp_err_t bsp_display_backlight_on(void);
// esp_err_t bsp_lcd_init(void);
// void lcd_set_color(uint16_t color);
// void lcd_draw_pictrue(int x_start, int y_start, int x_end, int y_end, const unsigned char *gImage);
/***************    LCD显示屏 ↑   *************************/
/***********************************************************/

