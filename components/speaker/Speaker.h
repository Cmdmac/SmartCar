#ifndef _SPEAKER_H
#define _SPEAKER_H
#include "Audio.h"

class Speaker {

    private:
        int bclk, lrc, dout;
        Audio audio;
        static void playDelegate(void *pvParameters) {
          Speaker* instance = static_cast<Speaker*>(pvParameters);
          while(1) {
            instance->loop();
            delay(1);
          }
        }
        void loop();
    public:
        Speaker(int bclk, int lrc, int dout) : bclk(bclk), lrc(lrc), dout(dout) {
            
        }
        void setup();
        void play(const char* source);


};

#endif