#include "Config.h"
#include "TFT_SPI.h"
#include "esp_camera.h"

#ifndef _CAMERA_H

class Camera {
    private:
        int d0, d1, d2, d3, d4, d5, d6, d7, xclk, pclk, vsync, href, siod, sioc, pwdn, reset;
        QueueHandle_t xQueueLCDFrame = NULL;
        TFT_SPI *tft = nullptr;

        static void task_process_lcd(void *arg);
        static void task_process_camera(void *arg);

    public:
        Camera(int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7, int xclk, int pclk, int vsync, int href, int siod, int sioc, int pwdn, int reset) 
        : d0(d0), d1(d1), d2(d2), d3(d3), d4(d4), d5(d5), d6(d6), d7(d7), xclk(xclk), pclk(pclk), vsync(vsync), href(href), siod(siod), sioc(sioc), pwdn(pwdn), reset(reset) {
            
        }
        bool setUp();
        bool setUp(TFT_SPI *tft) {
            if (!setUp()) {
                return false;
            }
            this->tft = tft;

            xQueueLCDFrame = xQueueCreate(2, sizeof(camera_fb_t *));
            xTaskCreatePinnedToCore(&Camera::task_process_camera, "task_process_camera", 3 * 1024, this, 5, NULL, 1);
            xTaskCreatePinnedToCore(&Camera::task_process_lcd, "task_process_lcd", 4 * 1024, this, 5, NULL, 0);
            return true;
        }
        void startStreamServer();

};
#endif