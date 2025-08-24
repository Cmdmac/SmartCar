#pragma once
#include <string.h>
#include "math.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_lcd_types.h"
#include <functional> // 包含std::function和std::bind


#include "Config.h"


#define BSP_LCD_DRAW_BUF_HEIGHT    (20)

// #define BSP_I2C_NUM           (0)            // I2C外设


class TFT_SPI {
    public:
        TFT_SPI(int mosi, int clk, int cs, int dc, int rst, int bl, bool bl_output_invert = false) {
            this->mosi = mosi;
            this->clk = clk;
            this->cs = cs;
            this->dc = dc;
            this->rst = rst;
            this->bl = bl;
            this->bl_output_invert = bl_output_invert;
        }
        void setup();
        // 立创esp32s3开发板将cs引脚放在了io扩展口上，在tft屏reset之后需要重新拉低cs引脚
        void setup(std::function<void()> callback) {
            onEnableCsPin = callback;
            setup();
        }
        esp_err_t setBrightness(int brightness);
        esp_err_t turnOnBacklight();
        esp_err_t turnOffBacklight();

        void fillScreen(uint16_t color);
        void drawPicture(int x_start, int y_start, int x_end, int y_end, const unsigned char *gImage);
        void drawPicture(const unsigned char *gImage);

    private:
        int mosi, clk, cs, dc, rst, bl, bl_output_invert;
        std::function<void()> onEnableCsPin = [](){};
        bool init();
        bool initLCD();
        #if USE_LVGL == true
            bool initLvgl();
        #endif // USE_LVGL
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


