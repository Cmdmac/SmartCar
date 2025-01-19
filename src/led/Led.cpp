#include "Led.h"
#include <Arduino.h>

Led::Led() : mBrightness(0), mFadeMount(10), mFlickerInterval(500) {

}

Led::Led(int pin) : mPin(pin), mBrightness(0), mFadeMount(10), mFlickerInterval(500) {
    pinMode(pin, OUTPUT);
}

void Led::setUp(int pin) {
    pinMode(pin, OUTPUT);
}

void Led::on() {
    digitalWrite(mPin, HIGH);
}

void Led::off() {
    digitalWrite(mPin, LOW);
}

void Led::setFlickerInterval(int interval) {
    mFlickerInterval = interval;
}

void Led::flicker() {
    on();
    delay(mFlickerInterval);
    off();
    delay(mFlickerInterval);
}

void Led::setFadeMount(int fadeMount) {
    mFadeMount = fadeMount;
}

void Led::setBrightness(int brightness) {
    mBrightness = brightness;
    analogWrite(mPin, mBrightness);
}

void Led::autoFade() {
    analogWrite(mPin, mBrightness);

    mBrightness += mFadeMount;
    if (mBrightness <= 0) {
        mBrightness = -mFadeMount;
        Serial.println(mBrightness);
        mFadeMount = -mFadeMount;
    } else if (mBrightness >= 255) {
        mBrightness = 255;
        mFadeMount = -mFadeMount;
    }
    delay(100);
}