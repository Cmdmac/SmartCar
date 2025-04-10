#include "Speaker.h"

// #define I2S_DOUT      15
// #define I2S_BCLK      16
// #define I2S_LRC       17

void Speaker::setup() {
    audio.setPinout(bclk, lrc, dout);
    audio.setVolume(18);
    xTaskCreatePinnedToCore(&Speaker::playDelegate, "SpeakerTask", 4096 * 2, this, 1, NULL, 0);
}

void Speaker::play(const char* source) {
    audio.connecttohost(source);
}

void Speaker::loop() {
    audio.loop();
}