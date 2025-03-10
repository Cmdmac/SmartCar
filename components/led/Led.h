#ifndef _LED_H
#define _LED_H

class Led {
    public:
        Led();
        Led(int pin);
        void setUp(int pin);
        void on();
        void off();
        void setFlickerInterval(int interval);
        void flicker();
        void setFadeMount(int fadeMount);
        void setBrightness(int brightness);
        void autoFade();

    private:
        int mPin;
        int mBrightness;
        int mFadeMount;
        int mFlickerInterval;
        int mMax;
        int mMin;


};

#endif