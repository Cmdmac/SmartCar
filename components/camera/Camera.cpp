#include "esp32-hal.h"
#include "camera.h"
#define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
#include "esp_camera.h"
// #include "camera_pins.h"
#include <Arduino.h>


void startCameraServer();

bool Camera::setUp() {

    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_1;
    config.ledc_timer = LEDC_TIMER_1;
    config.pin_d0 = d0;
    config.pin_d1 = d1;
    config.pin_d2 = d2;
    config.pin_d3 = d3;
    config.pin_d4 = d4;
    config.pin_d5 = d5;
    config.pin_d6 = d6;
    config.pin_d7 = d7;
    config.pin_xclk = xclk;
    config.pin_pclk = pclk;
    config.pin_vsync = vsync;
    config.pin_href = href;
    config.pin_sccb_sda = -1;
    config.pin_sccb_scl = sioc;
    config.sccb_i2c_port = 0;
    config.pin_pwdn = pwdn;
    config.pin_reset = reset;
    config.xclk_freq_hz = 24000000;
    config.frame_size = FRAMESIZE_QCIF;
    config.pixel_format = PIXFORMAT_RGB565; // for streaming
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    // config.jpeg_quality = 12;
    config.fb_count = 2;
    // config.fb_location = CAMERA_FB_IN_PSRAM;
    // config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
    // for larger pre-allocated frame buffer.
    // if(psramFound()){
    //     Serial.println("psramFound");
    //     config.jpeg_quality = 10;
    //     config.fb_count = 2;
    //     config.grab_mode = CAMERA_GRAB_LATEST;
    // } else {
    //     // Limit the frame size when PSRAM is not available
    //     Serial.println("no psram");
    //     config.frame_size = FRAMESIZE_SVGA;
    //     config.fb_location = CAMERA_FB_IN_DRAM;
    // }

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return false;
    }

    // sensor_t * s = esp_camera_sensor_get();
    // initial sensors are flipped vertically and colors are a bit saturated
    // s->set_vflip(s, 1); // flip it back
    // s->set_brightness(s, 1); // up the brightness just a bit
    // s->set_saturation(s, 0); // lower the saturation
    return true;
}

void Camera::startStreamServer() {
    Serial.println("startStreamServer");
    ::startCameraServer();
}


// lcd处理任务
void Camera::task_process_lcd(void *arg)
{
    Camera* instance = static_cast<Camera*>(arg);
    camera_fb_t *frame = NULL;

    while (true)
    {
        if (xQueueReceive(instance->xQueueLCDFrame, &frame, portMAX_DELAY))
        {
            // esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, frame->width, frame->height, (uint16_t *)frame->buf);
            instance->tft->drawPicture(0, 0, frame->width, frame->height, frame->buf);
            esp_camera_fb_return(frame);
        }
        vTaskDelay(pdMS_TO_TICKS(1)); 
    }
}

// 摄像头处理任务
void Camera::task_process_camera(void *arg)
{
    Camera* instance = static_cast<Camera*>(arg);
    while (true)
    {
        camera_fb_t *frame = esp_camera_fb_get();
        if (frame) {
            xQueueSend(instance->xQueueLCDFrame, &frame, portMAX_DELAY);
            ESP_LOGI("Camera", "Captured frame: %d bytes, width: %d, height: %d, format: %d",
                     frame->len, frame->width, frame->height, frame->format);
        } else {
            // ESP_LOGE("Camera", "Failed to capture frame");
        }
        vTaskDelay(pdMS_TO_TICKS(1)); 
    }
}

