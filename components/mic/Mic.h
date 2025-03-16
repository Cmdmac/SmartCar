#ifndef _MIC_H_
#define _MIC_H_
#include <ESP_I2S.h>

#include <Arduino.h>

// #define I2S_IN_BCLK 40
// #define I2S_IN_LRC 42
// #define I2S_IN_DIN 2

// SCK（BCLK）	GPIO4
// WS（LRC）	GPIO5
// SD（DIN）	GPIO6

// 定义 INMP441 连接的 ESP32S3 引脚
// INMP441 config
// #define I2S_IN_PORT I2S_NUM_0
#define I2S_IN_BCLK 5//40 // SCK
#define I2S_IN_LRC 4//42 // WS
#define I2S_IN_DIN 6//41 // SD
#define RECORD_TIME   10  // seconds, The maximum value is 240
// #define SAMPLE_RATE 16000
// #define BIT_WIDTH I2S_DATA_BIT_WIDTH_32BIT

typedef std::function<void(uint8_t*, size_t)> MicCallback;

class Mic {
    public:
        void setup(int pin_bclk, int pin_lrc, int pin_din, int sampleRate, i2s_data_bit_width_t bit_width);
        void setup(int sampleRate, i2s_data_bit_width_t bit_width);
        void recordWav(int recordTime, MicCallback callback);
        size_t read(char *buffer, size_t size);

        void loop();
    private:
        // int mSampleRate;
        // 创建一个 I2SClass 实例
        I2SClass i2s;
};

#endif

