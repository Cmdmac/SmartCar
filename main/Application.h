#pragma once

class Application {
    public:
        Application() {
            // Serial.println("Application");
        }
        ~Application() {
            // Serial.println("~Application");
        }
        void setup();
        void powerOn();
        void powerOff();
        
        void loop();
};