#include "TFT_SPI.h"

#ifdef DRIVER_GC9A01
#include "esp_lcd_gc9a01.h"
#endif
/***********************************************************/
/****************    LCD显示屏 ↓   *************************/

const char* TAG = "TFT_SPI";
// 背光PWM初始化
esp_err_t TFT_SPI::init_brightness(void)
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

// 液晶屏初始化
bool TFT_SPI::init(void)
{
    esp_lcd_panel_io_handle_t io_handle = NULL; 
    esp_err_t ret = ESP_OK;
    // 背光初始化
    if(ESP_OK != init_brightness()) {   
        ESP_LOGI(TAG, "Brightness init failed");
        return false;
    }
    // 初始化SPI总线
    ESP_LOGD(TAG, "Initialize SPI bus");
    // const spi_bus_config_t buscfg = {
    //     .sclk_io_num = BSP_LCD_SPI_CLK,
    //     .mosi_io_num = BSP_LCD_SPI_MOSI,
    //     .miso_io_num = GPIO_NUM_NC,
    //     .quadwp_io_num = GPIO_NUM_NC,
    //     .quadhd_io_num = GPIO_NUM_NC,
    //     .max_transfer_sz = BSP_LCD_H_RES * BSP_LCD_V_RES * sizeof(uint16_t),
    // };
    const spi_bus_config_t buscfg = createBusConfig();
    if (ESP_OK != spi_bus_initialize(BSP_LCD_SPI_NUM, &buscfg, SPI_DMA_CH_AUTO)) {
        ESP_LOGI(TAG, "SPI init failed");
        return false;
    }
    // 液晶屏控制IO初始化
    ESP_LOGD(TAG, "Install panel IO");
    // const esp_lcd_panel_io_spi_config_t io_config = {
    //     .dc_gpio_num = BSP_LCD_DC,
    //     .cs_gpio_num = BSP_LCD_SPI_CS,
    //     .pclk_hz = BSP_LCD_PIXEL_CLOCK_HZ,
    //     .lcd_cmd_bits = LCD_CMD_BITS,
    //     .lcd_param_bits = LCD_PARAM_BITS,
    //     .spi_mode = 2,
    //     .trans_queue_depth = 10,
    // };
        // esp_lcd_panel_io_handle_t io_handle = NULL;
    const esp_lcd_panel_io_spi_config_t io_config = createIoConfig();
    if(ESP_OK != esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)BSP_LCD_SPI_NUM, &io_config, &io_handle)) {
        if (panel_handle) {
                esp_lcd_panel_del(panel_handle);
        }
        if (io_handle) {
            esp_lcd_panel_io_del(io_handle);
        }
        spi_bus_free(BSP_LCD_SPI_NUM);
        
        ESP_LOGI(TAG, "New panel IO failed");
        return false;
    }
    // 初始化液晶屏驱动芯片ST7789
    ESP_LOGD(TAG, "Install LCD driver");
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = BSP_LCD_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
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
    #endif
    
    esp_lcd_panel_reset(panel_handle);  // 液晶屏复位
    // lcd_cs(0);  // 拉低CS引脚
    esp_lcd_panel_init(panel_handle);  // 初始化配置寄存器
    esp_lcd_panel_invert_color(panel_handle, true); // 颜色反转
    esp_lcd_panel_swap_xy(panel_handle, true);  // 显示翻转 
    esp_lcd_panel_mirror(panel_handle, true, false); // 镜像

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

spi_bus_config_t TFT_SPI::createBusConfig() {
    spi_bus_config_t config;
    #ifdef DRIVER_GC9A01
        config =GC9A01_PANEL_BUS_SPI_CONFIG(BSP_LCD_SPI_CLK, BSP_LCD_SPI_MOSI,
                                    BSP_LCD_H_RES * 80 * BSP_LCD_V_RES / 8);
    #endif
    return config;
}
esp_lcd_panel_io_spi_config_t TFT_SPI::createIoConfig() {
    esp_lcd_panel_io_spi_config_t io_config;
    #ifdef DRIVER_GC9A01
        io_config = GC9A01_PANEL_IO_SPI_CONFIG(BSP_LCD_SPI_CS, BSP_LCD_DC,
                NULL, NULL);
    #endif        
    return io_config;
}

// #define DRIVER_GC9A01
// bool TFT_SPI::initGC9A01() {

// }
// #endif

// LCD显示初始化
void TFT_SPI::setup(void)
{
    esp_err_t ret = ESP_OK;
    ESP_LOGI(TAG, "bsp_lcd_init");
    
    ret = init(); // 液晶屏驱动初始化
    ESP_LOGI(TAG, "bsp_display_new");

    setBackgroundColor(0x0000); // 设置整屏背景黑色
    ret = esp_lcd_panel_disp_on_off(panel_handle, true); // 打开液晶屏显示
    // ret = bsp_display_backlight_on(); // 打开背光显示
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