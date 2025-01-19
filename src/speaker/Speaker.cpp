#include "Speaker.h"

// #define I2S_DOUT      15
// #define I2S_BCLK      16
// #define I2S_LRC       17

#define I2S_DOUT      42
#define I2S_BCLK      2
#define I2S_LRC       1

void Speaker::setup() {
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(18);
    xTaskCreatePinnedToCore(&Speaker::playDelegate, "SpeakerTask", 4096 * 2, this, 1, NULL, 0);
}

void Speaker::play(const char* source) {
    audio.connecttohost(source);
}

void Speaker::loop() {
    audio.loop();
}