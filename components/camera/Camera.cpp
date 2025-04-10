#include "esp32-hal.h"
#include "camera.h"
#define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
#include "esp_camera.h"
#include "camera_pins.h"
#include <Arduino.h>


void startCameraServer();

void Camera::setUp() {

    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = y2;
    config.pin_d1 = y3;
    config.pin_d2 = y4;
    config.pin_d3 = y5;
    config.pin_d4 = y6;
    config.pin_d5 = y7;
    config.pin_d6 = y8;
    config.pin_d7 = y9;
    config.pin_xclk = xclk;
    config.pin_pclk = pclk;
    config.pin_vsync = vsync;
    config.pin_href = href;
    config.pin_sccb_sda = siod;
    config.pin_sccb_scl = sioc;
    config.pin_pwdn = pwdn;
    config.pin_reset = reset;
    config.xclk_freq_hz = 12000000;
    config.frame_size = FRAMESIZE_UXGA;
    config.pixel_format = PIXFORMAT_JPEG; // for streaming
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
    // for larger pre-allocated frame buffer.
    if(psramFound()){
        Serial.println("psramFound");
        config.jpeg_quality = 10;
        config.fb_count = 2;
        config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
        // Limit the frame size when PSRAM is not available
        Serial.println("no psram");
        config.frame_size = FRAMESIZE_SVGA;
        config.fb_location = CAMERA_FB_IN_DRAM;
    }

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    sensor_t * s = esp_camera_sensor_get();
    // initial sensors are flipped vertically and colors are a bit saturated
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, 0); // lower the saturation
}

void Camera::startStreamServer() {
    Serial.println("startStreamServer");
    ::startCameraServer();
}