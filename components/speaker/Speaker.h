#ifndef _SPEAKER_H
#define _SPEAKER_H
#include <Arduino.h>

// #include <WiFiClientSecure.h>
// #include <libmad.h>
class Speaker {

    private:
        int bclk, lrc, dout;
        // Audio audio;
        static void playDelegate(void *pvParameters) {
          Speaker* instance = static_cast<Speaker*>(pvParameters);

          while(1) {
            // instance->loop();
            // Serial.println("Speaker loop running");

            delay(1);
          }
        }
        void loop();

        static const char* TAG;
        static const size_t AUDIO_BUFFER_SIZE = 8192;
        static const size_t OUTPUT_BUFFER_SIZE = 4096;
        
        // StreamCopy copier;  // 用于流复制
        bool is_playing;
    public:
        Speaker(int bclk, int lrc, int dout) : bclk(bclk), lrc(lrc), dout(dout),
          // copier(audioOutput, urlStream),  // 初始化流复制器
          is_playing(false) {

        }
        void setup();
        void play(const char* source);
        void stop();
        void setVolume(float volume);

};

#endif