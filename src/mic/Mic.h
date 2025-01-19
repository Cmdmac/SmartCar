#ifndef _MIC_H_
#define _MIC_H_
#include <Arduino.h>
#include "driver/i2s_std.h"


#define SAMPLE_RATE 44100
#define BITS_PER_SAMPLE I2S_BITS_PER_SAMPLE_16BIT
#define I2S_CHANNEL I2S_CHANNEL_MONO
#define I2S_BUFFER_SIZE 1024
#define WAV_HEADER_SIZE 44
#define SAMPLE_BITS 16


// #define I2S_IN_BCLK 40
// #define I2S_IN_LRC 42
// #define I2S_IN_DIN 2

// 定义 INMP441 连接的 ESP32S3 引脚
// INMP441 config
#define I2S_IN_PORT I2S_NUM_0
#define I2S_IN_BCLK 40 // SCK
#define I2S_IN_LRC 42 // WS
#define I2S_IN_DIN 41 // SD
#define RECORD_TIME   10  // seconds, The maximum value is 240

class Mic {
    public:
        
        void setup(int pinWS, int pinSD, int pinSCK);
        void recordWav(const char* url, int recordTime, int sampleRate, int sampleBits);

        void loop();
    private:
        void generateWavHeader(uint8_t *wav_header, uint32_t wav_size, uint32_t sample_rate);
        // int mSampleRate;
        // int mSampleBits;
};

#endif

