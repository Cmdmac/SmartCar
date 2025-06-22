#pragma once
#include <string.h>
#include "math.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_lcd_types.h"


#include "Config.h"


#define BSP_LCD_DRAW_BUF_HEIGHT    (20)

// #define BSP_I2C_NUM           (0)            // I2C外设


class TFT_SPI {
    public:
        TFT_SPI(int mosi, int clk, int cs, int dc, int rst, int bl) {
            this->mosi = mosi;
            this->clk = clk;
            this->cs = cs;
            this->dc = dc;
            this->rst = rst;
            this->bl = bl;
        }
        void setup();
        esp_err_t setBrightness(int brightness);
        esp_err_t turnOnBacklight();
        esp_err_t turnOffBacklight();
        void fillScreen(uint16_t color);
        void drawPicture(int x_start, int y_start, int x_end, int y_end, const unsigned char *gImage);
        void drawPicture(const unsigned char *gImage);

    private:
        int mosi, clk, cs, dc, rst, bl;
        bool init();
        bool initLCD();
        bool initLvgl();
        #if TFT_HAS_TOUCH == true
            bool initTouch(lv_disp_t*);
        #endif
        // lv_indev_t *initTouch(lv_disp_t *disp);
        // esp_err_t initTouchDriver(esp_lcd_touch_handle_t *ret_touch);

        esp_err_t initBrightness();

        esp_lcd_panel_handle_t panel_handle = NULL;
        esp_lcd_panel_io_handle_t io_handle = NULL;
        // esp_lcd_touch_handle_t tp;   // 触摸屏句柄
        
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

