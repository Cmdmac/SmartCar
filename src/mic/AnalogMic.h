#ifndef _ANOLOG_MIC_H_
#define _ANOLOG_MIC_H_
#include <SPI.h>
#include <SD.h>
#include <SPIFFS.h>

// 定义采样率（单位：Hz），这里设置为比较常用的8000Hz，可根据需求调整
const int sampleRate = 8000;
// 每个采样点的位数，8位表示范围是0-255
const int bitsPerSample = 8;
// 定义模拟输入引脚，这里连接咪头信号输出
const int audioPin = A0;

// WAV文件头结构体定义
typedef struct {
  char chunkId[4];
  int chunkSize;
  char format[4];
  char subChunk1Id[4];
  int subChunk1Size;
  short int audioFormat;
  short int numChannels;
  int sampleRate;
  int byteRate;
  short int blockAlign;
  short int bitsPerSample;
  char subChunk2Id[4];
  int subChunk2Size;
} WAVHeader;


class AnalogMic
{
private:
    /* data */
    
    void initWAVHeader(WAVHeader &header, int dataSize);
    void intToByteArray(int value, unsigned char *array, int startIndex);
public:
    // AnalogMic(/* args */);
    // ~AnalogMic();

    void setup();
    void record(const char* spiffsPath, long time);
};

#endif