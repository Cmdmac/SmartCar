#ifndef _MIC_H_
#define _MIC_H_
#include <ESP_I2S.h>

#include <Arduino.h>

#define SAMPLE_RATE 44100
#define BITS_PER_SAMPLE I2S_BITS_PER_SAMPLE_16BIT
#define I2S_CHANNEL I2S_CHANNEL_MONO
#define I2S_BUFFER_SIZE 1024
#define WAV_HEADER_SIZE 44
#define SAMPLE_BITS 16


// #define I2S_IN_BCLK 40
// #define I2S_IN_LRC 42
// #define I2S_IN_DIN 2

// SCK（BCLK）	GPIO4
// WS（LRC）	GPIO5
// SD（DIN）	GPIO6

// 定义 INMP441 连接的 ESP32S3 引脚
// INMP441 config
#define I2S_IN_PORT I2S_NUM_0
#define I2S_IN_BCLK 4//40 // SCK
#define I2S_IN_LRC 5//42 // WS
#define I2S_IN_DIN 6//41 // SD
#define RECORD_TIME   10  // seconds, The maximum value is 240
#define SAMPLE_RATE 16000
#define BIT_WIDTH I2S_DATA_BIT_WIDTH_32BIT

typedef std::function<void(uint8_t*, size_t)> MicCallback;

class Mic {
    public:
        
        void setup();
        void recordWav(int recordTime, MicCallback callback);
        size_t read(char *buffer, size_t size);

        void loop();
    private:
        // int mSampleRate;
        // 创建一个 I2SClass 实例
        I2SClass i2s;
};

#endif

