#pragma once
#include "iBeacon.h"
#include "Protocol.h"
#include "QMC5883LCompass.h"

const char* ssid     = "Stark";  // 替换为您的 Wi-Fi 网络名称
const char* password = "fengzhiping,1101";  // 替换为您的 Wi-Fi 密码

class Application {
    public:
        Application() {
            // Serial.println("Application");
        }
        ~Application() {
            // Serial.println("~Application");
        }
        void setup();
        void setUpWifi();
        void setupBeacon();
        void setupCompass();
        void powerOn();
        void powerOff();

        void scanI2CDevices();
        
        void loop();

    private:
        Protocol net;
        QMC5883LCompass compass;
        iBeaconFinder finder;

        void reportLocation(Protocol net);
        static void beaconDelegate(void *pvParameters) {
            Application* instance = static_cast<Application*>(pvParameters);
            while(1) {
                // instance->loop();
                // Serial.println("Application loop running");
                instance->reportLocation(instance->net);
                delay(1000);
            }
        }

        void compassTask();
        static void compaseDelegate(void *pvParameters) {
            Application* instance = static_cast<Application*>(pvParameters);
            while(1) {
                // instance->loop();
                // Serial.println("Application loop running");
                instance->compassTask();
                delay(1000);
            }
        }
};