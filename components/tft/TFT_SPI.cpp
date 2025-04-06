#include "TFT_SPI.h"
#include "driver/gpio.h"
#include "Arduino.h"
#ifdef DRIVER_GC9A01
#include "esp_lcd_gc9a01.h"
#endif

// #include "esp_lcd_touch_ft5x06.h"
/***********************************************************/
/****************    LCD显示屏 ↓   *************************/

const char* TAG = "TFT_SPI";
// 背光PWM初始化
esp_err_t TFT_SPI::initBrightness(void)
{
    // Setup LEDC peripheral for PWM backlight control
    const ledc_channel_config_t LCD_backlight_channel = {
        .gpio_num = BSP_LCD_BACKLIGHT,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LCD_LEDC_CH,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_1,
        .duty = 0,
        .hpoint = 0,
        // .flags.output_invert = true
    };
    // LCD_backlight_channel.flags.output_invert = true;

    const ledc_timer_config_t LCD_backlight_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = LEDC_TIMER_1,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ESP_ERROR_CHECK(ledc_timer_config(&LCD_backlight_timer));
    ESP_ERROR_CHECK(ledc_channel_config(&LCD_backlight_channel));

    return ESP_OK;
}

// 背光亮度设置
esp_err_t TFT_SPI::setBrightness(int brightness)
{
    if (brightness > 100) {
        brightness = 100;
    } else if (brightness < 0) {
        brightness = 0;
    }

    ESP_LOGI(TAG, "Setting LCD backlight: %d%%", brightness);
    // LEDC resolution set to 10bits, thus: 100% = 1023
    uint32_t duty_cycle = (1023 * brightness) / 100;
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH, duty_cycle));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH));

    return ESP_OK;
}

// 关闭背光
esp_err_t TFT_SPI::turnOffBacklight(void)
{
    return setBrightness(0);
}

// 打开背光 最亮
esp_err_t TFT_SPI::turnOnBacklight(void)
{
    return setBrightness(100);
}

#define BSP_I2C_NUM           I2C_NUM_0             // I2C外设
#define BSP_I2C_FREQ_HZ       100000         // 100kHz

// esp_err_t bsp_i2c_init(void)
// {
//     i2c_config_t i2c_conf = {
//         .mode = I2C_MODE_MASTER,
//         .sda_io_num = 14,
//         .scl_io_num = 13,
//         .sda_pullup_en = GPIO_PULLUP_DISABLE,
//         .scl_pullup_en = GPIO_PULLUP_DISABLE,
//         // .clk_speed = BSP_I2C_FREQ_HZ
//     };
//     i2c_conf.master.clk_speed = BSP_I2C_FREQ_HZ;
//     i2c_param_config(BSP_I2C_NUM, &i2c_conf);

//     return i2c_driver_install(BSP_I2C_NUM, i2c_conf.mode, 0, 0, 0);
// }

// 液晶屏初始化
bool TFT_SPI::init(void)
{
    //初始化LCD
    initLCD();
    //初始化触摸屏
    // initLvgl();
    return true;

// err:
//     if (panel_handle) {
//         esp_lcd_panel_del(panel_handle);
//     }
//     if (io_handle) {
//         esp_lcd_panel_io_del(io_handle);
//     }
//     spi_bus_free(BSP_LCD_SPI_NUM);
    // return ret;
}

bool TFT_SPI::initLCD() {
    initBrightness();
    // gpio_config_t lcd_bl_io_conf = {
    //     .pin_bit_mask = (1ULL << BSP_LCD_BACKLIGHT),  // Select the GPIO pin
    //     .mode = GPIO_MODE_OUTPUT,            // Set as output mode
    //     .pull_up_en = GPIO_PULLUP_DISABLE,
    //     .pull_down_en = GPIO_PULLDOWN_DISABLE,
    //     .intr_type = GPIO_INTR_DISABLE       // Disable interrupt
    // };
    // gpio_config(&lcd_bl_io_conf);
    // gpio_set_level((BSP_LCD_BACKLIGHT), 1);


    #if defined(DRIVER_GC9A01)
        spi_bus_config_t bus_conf =GC9A01_PANEL_BUS_SPI_CONFIG(BSP_LCD_SPI_CLK, BSP_LCD_SPI_MOSI,
                                    BSP_LCD_H_RES * 80 * BSP_LCD_V_RES / 8);
    #elif defined(DRIVER_ST7789)
        spi_bus_config_t bus_conf = {
            .mosi_io_num = BSP_LCD_SPI_MOSI,
            .miso_io_num = GPIO_NUM_NC,
            .sclk_io_num = BSP_LCD_SPI_CLK,
            .quadwp_io_num = GPIO_NUM_NC,
            .quadhd_io_num = GPIO_NUM_NC,
            .max_transfer_sz = BSP_LCD_H_RES * BSP_LCD_V_RES * sizeof(uint16_t),
        };
    #endif
    
    ESP_ERROR_CHECK(spi_bus_initialize(BSP_LCD_SPI_NUM, &bus_conf, SPI_DMA_CH_AUTO));

    ESP_LOGI(TAG, "Install panel IO");
    // esp_lcd_panel_io_handle_t io_handle = NULL;
    #ifdef DRIVER_GC9A01
        esp_lcd_panel_io_spi_config_t io_config = GC9A01_PANEL_IO_SPI_CONFIG(BSP_LCD_SPI_CS, BSP_LCD_DC,
                NULL, NULL);
    #elif defined(DRIVER_ST7789)
        esp_lcd_panel_io_spi_config_t io_config = {
            .cs_gpio_num = BSP_LCD_SPI_CS,
            .dc_gpio_num = BSP_LCD_DC,
            .spi_mode = 0,
            .pclk_hz = BSP_LCD_PIXEL_CLOCK_HZ,
            .trans_queue_depth = 10,
            .lcd_cmd_bits = LCD_CMD_BITS,
            .lcd_param_bits = LCD_PARAM_BITS,
            .flags = {
                .sio_mode = 0
            }
        };
    #endif
    // Attach the LCD to the SPI bus
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)BSP_LCD_SPI_NUM, &io_config, &io_handle));

    // ESP_LOGI(TAG, "Install ST7789 panel driver");
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = BSP_LCD_RST,
        .rgb_endian = LCD_RGB_ENDIAN_RGB,
        .bits_per_pixel = BSP_LCD_BITS_PER_PIXEL,
    };
    
    #ifdef DRIVER_GC9A01
        if (ESP_OK != esp_lcd_new_panel_gc9a01(io_handle, &panel_config, &panel_handle)) {
            if (panel_handle) {
                esp_lcd_panel_del(panel_handle);
            }
            if (io_handle) {
                esp_lcd_panel_io_del(io_handle);
            }
            spi_bus_free(BSP_LCD_SPI_NUM);
            ESP_LOGI(TAG, "New panel failed");
            return false;
        }
    #elif defined(DRIVER_ST7789)
        if (ESP_OK != esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle)) {
            if (panel_handle) {
                esp_lcd_panel_del(panel_handle);
            }
            if (io_handle) {
                esp_lcd_panel_io_del(io_handle);
            }
            spi_bus_free(BSP_LCD_SPI_NUM);
            ESP_LOGI(TAG, "New panel failed");
            return false;
        }
    #endif
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));// Set inversion for esp32s3eye

    // turn on display
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
    // esp_lcd_panel_reset(panel_handle);  // 液晶屏复位
    // esp_lcd_panel_init(panel_handle);  // 初始化配置寄存器
    // esp_lcd_panel_invert_color(panel_handle, true); // 颜色反转
    // esp_lcd_panel_swap_xy(panel_handle, false);  // 显示翻转 
    // esp_lcd_panel_mirror(panel_handle, true, false); // 镜像

    ESP_LOGI(TAG, "turn on lcd");
    // setBackgroundColor(0x000); // 设置整屏背景黑色
    // turnOnBacklight(); // 打开背光显示
    return true;
}

bool TFT_SPI::initLvgl() {
    /* 液晶屏添加LVGL接口 */
    /* 初始化LVGL */
    lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    lvgl_port_init(&lvgl_cfg);
    ESP_LOGD(TAG, "Add LCD screen");
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = BSP_LCD_H_RES * BSP_LCD_DRAW_BUF_HEIGHT,   // LVGL缓存大小 
        .double_buffer = false, // 是否开启双缓存
        .hres = BSP_LCD_H_RES, // 液晶屏的宽
        .vres = BSP_LCD_V_RES, // 液晶屏的高
        .monochrome = false,  // 是否单色显示器
        /* Rotation的值必须和液晶屏初始化里面设置的 翻转 和 镜像 一样 */
        .rotation = {
            .swap_xy = false,  // 是否翻转
            .mirror_x = false, // x方向是否镜像
            .mirror_y = true, // y方向是否镜像
        },
        .flags = {
            .buff_dma = false,  // 是否使用DMA 注意：dma与spiram不能同时为true
            .buff_spiram = true, // 是否使用PSRAM 注意：dma与spiram不能同时为true
        }
    };

    lv_disp_t *disp = lvgl_port_add_disp(&disp_cfg);

    //初始化触摸屏
    /* Initilize I2C */
    /**/
    // i2c_config_t i2c_conf = {
    //     .mode = I2C_MODE_MASTER,
    //     .sda_io_num = GPIO_NUM_14,
    //     .scl_io_num = GPIO_NUM_13,
    //     .sda_pullup_en = GPIO_PULLUP_ENABLE,
    //     .scl_pullup_en = GPIO_PULLUP_ENABLE,
    //     // .master.clk_speed = 400000
    // };
    // i2c_conf.master.clk_speed = 400000;

    // ESP_ERROR_CHECK(i2c_param_config(BSP_I2C_NUM, &i2c_conf));
    // ESP_ERROR_CHECK(i2c_driver_install(BSP_I2C_NUM, i2c_conf.mode, 0, 0, 0));

    // i2c_cmd_handle_t cmd;
    // for (int i = 0; i < 0x7f; i++)
    // {
    //     cmd = i2c_cmd_link_create();
    //     i2c_master_start(cmd);
    //     i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, true);
    //     i2c_master_stop(cmd);
    //     if (i2c_master_cmd_begin(BSP_I2C_NUM, cmd, portMAX_DELAY) == ESP_OK)  
    //     {
    //         ESP_LOGW("I2C_TEST", "%02X", i);
    //     }
    //     i2c_cmd_link_delete(cmd);
    // }

    // initTouch(disp);
    return true;
}

bool TFT_SPI::initTouch(lv_disp_t* disp) {
    /* Initialize touch */
    esp_lcd_touch_config_t tp_cfg = {
        .x_max = BSP_LCD_V_RES,
        .y_max = BSP_LCD_H_RES,
        .rst_gpio_num = GPIO_NUM_3, // Shared with LCD reset
        .int_gpio_num = GPIO_NUM_40, 
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = false,
            .mirror_x = 0,
            .mirror_y = true,
        },
    };
    ESP_LOGE(TAG, "init touch");
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_CST816S_CONFIG();

    esp_lcd_touch_handle_t touch_handler = NULL;

    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)BSP_I2C_NUM, &tp_io_config, &tp_io_handle), TAG, "esp_lcd_new_panel_io_i2c init failure");
    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_cst816s(tp_io_handle, &tp_cfg, &touch_handler));

    /* 添加LVGL接口 */
    const lvgl_port_touch_cfg_t touch_cfg = {
        .disp = disp,
        .handle = touch_handler,
    };

    lvgl_port_add_touch(&touch_cfg);
    return true;
}

// LCD显示初始化
void TFT_SPI::setup(void)
{
    esp_err_t ret = ESP_OK;
    ESP_LOGI(TAG, "init tft lcd");    
    bool r = init(); // 液晶屏驱动初始
    ret = turnOnBacklight(); // 打开背光显示
    // test_draw_bitmap(panel_handle);

    return;
}

// 显示图片
void TFT_SPI::drawPicture(int x_start, int y_start, int x_end, int y_end, const unsigned char *gImage)
{
    // 分配内存 分配了需要的字节大小 且指定在外部SPIRAM中分配
    size_t pixels_byte_size = (x_end - x_start)*(y_end - y_start) * 2;
    uint16_t *pixels = (uint16_t *)heap_caps_malloc(pixels_byte_size, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
    if (NULL == pixels)
    {
        ESP_LOGE(TAG, "Memory for bitmap is not enough");
        return;
    }
    memcpy(pixels, gImage, pixels_byte_size);  // 把图片数据拷贝到内存
    esp_lcd_panel_draw_bitmap(panel_handle, x_start, y_start, x_end, y_end, (uint16_t *)pixels); // 显示整张图片数据
    heap_caps_free(pixels);  // 释放内存
}

// 设置液晶屏颜色
void TFT_SPI::setBackgroundColor(uint16_t color)
{
    // 分配内存 这里分配了液晶屏一行数据需要的大小
    uint16_t *buffer = (uint16_t *)heap_caps_malloc(BSP_LCD_H_RES * sizeof(uint16_t), MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
    
    if (NULL == buffer)
    {
        ESP_LOGE(TAG, "Memory for bitmap is not enough");
    }
    else
    {
        for (size_t i = 0; i < BSP_LCD_H_RES; i++) // 给缓存中放入颜色数据
        {
            buffer[i] = color;
        }
        for (int y = 0; y < BSP_LCD_V_RES; y++) // 显示整屏颜色
        {
            esp_lcd_panel_draw_bitmap(panel_handle, 0, y, BSP_LCD_H_RES, y+1, buffer);
        }
        free(buffer); // 释放内存
    }
}
/***************    LCD显示屏 ↑   *************************/
/***********************************************************/