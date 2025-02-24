#ifndef _SPEAKER_H
#define _SPEAKER_H
#include "Audio.h"

class Speaker {

    private:
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
        void setup();
        void play(const char* source);


};

#endif