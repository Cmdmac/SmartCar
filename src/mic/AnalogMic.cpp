#include "AnalogMic.h"


// 初始化WAV文件头
void AnalogMic::initWAVHeader(WAVHeader &header, int dataSize) {
  strcpy(header.chunkId, "RIFF");
  header.chunkSize = 36 + dataSize;
  strcpy(header.format, "WAVE");
  strcpy(header.subChunk1Id, "fmt ");
  header.subChunk1Size = 16;
  header.audioFormat = 1;
  header.numChannels = 1;
  header.sampleRate = sampleRate;
  header.byteRate = sampleRate * (bitsPerSample / 8);
  header.blockAlign = bitsPerSample / 8;
  header.bitsPerSample = bitsPerSample;
  strcpy(header.subChunk2Id, "data");
  header.subChunk2Size = dataSize;
}

// 将整数转换为字节数组（小端序）
void AnalogMic::intToByteArray(int value, byte *array, int startIndex) {
  array[startIndex] = value & 0xFF;
  array[startIndex + 1] = (value >> 8) & 0xFF;
  array[startIndex + 2] = (value >> 16) & 0xFF;
  array[startIndex + 3] = (value >> 24) & 0xFF;
}

// 初始化SPIFFS文件系统
void AnalogMic::setup() {
  Serial.begin(9600);
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialization failed!");
    while (1);
  }
  Serial.println("SPIFFS initialized successfully");
}

// 主循环函数
void AnalogMic::record(const char* spiffsPath, long time) {
  // 打开要写入的WAV文件
  File audioFile = SPIFFS.open(spiffsPath, "w");
  if (!audioFile) {
    Serial.println("Failed to open file for writing");
    return;
  }

  // 初始化WAV文件头结构体
  WAVHeader header;
  initWAVHeader(header, 0);  // 初始时数据大小设为0，后面再更新

  // 将WAV文件头写入文件
  audioFile.write((byte *)&header, sizeof(WAVHeader));

  const int sampleInterval = 1000000 / sampleRate;  // 计算采样间隔（微秒）
  unsigned long lastSampleTime = micros();  // 记录上次采样时间
  int numSamples = 0;  // 记录采样点数

  // 开始录音循环
  while (true) {
    if (micros() - lastSampleTime >= sampleInterval) {
      int sample = analogRead(audioPin);  // 读取模拟输入引脚的值
      byte audioByte = map(sample, 0, 1023, 0, 255);  // 将0-1023范围映射到0-255范围
      audioFile.write(audioByte);  // 将采样数据写入文件
      numSamples++;
      lastSampleTime = micros();
    }
    // 可以添加一个停止录音的条件判断，比如按下某个按钮等，这里简单设置为采样一定点数后停止
    if (numSamples >= sampleRate * 10) {  // 这里录制5秒音频，可根据需求调整时长
      break;
    }
  }

  // 更新WAV文件头中的数据大小信息
  header.subChunk2Size = numSamples * (bitsPerSample / 8);
  header.chunkSize = 36 + header.subChunk2Size;
  audioFile.seek(0);  // 回到文件开头
  audioFile.write((byte *)&header, sizeof(WAVHeader));  // 重写文件头

  audioFile.close();
  Serial.println("Recording finished and saved as audio.wav");
  while (1);  // 录音结束后停止在这里，可以根据需要修改后续行为
}