#include "Mic.h"
// #include "net.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"


// extern Net net;
/*
uint8_t i2s_buffer[I2S_BUFFER_SIZE];  // 音频数据缓冲区
static void i2s_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == I2S_EVENT && event_id == I2S_EVENT_RX_DONE) {
        size_t bytes_read = 0;
        esp_err_t err = i2s_read(I2S_NUM, i2s_buffer, I2S_BUFFER_SIZE, &bytes_read, portMAX_DELAY);
        if (err == ESP_OK) {
            // 将读取到的音频数据写入文件
            if (audio_file!= NULL) {
                fwrite(i2s_buffer, 1, bytes_read, audio_file);
            }
            ESP_LOGI(TAG, "Read %d bytes from I2S", bytes_read);
        } else {
            ESP_LOGE(TAG, "Error reading from I2S: %d", err);
        }
    }
}
*/
void Mic::setup() {
  Serial.println("AudioRecorder Setup I2S...");
  
  Serial.println("正在初始化 I2S 总线...");
  // 设置用于音频输入的引脚
  // Set up the pins used for audio input
  i2s.setPins(I2S_IN_BCLK, I2S_IN_LRC, -1, I2S_IN_DIN);

  // 以 16 kHz 频率及 16 位深度单声道启动 I2S
  if (!i2s.begin(I2S_MODE_STD, 16000, I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_MONO, I2S_STD_SLOT_LEFT)) {
    Serial.println("Failed to initialize I2S bus!");
    return;
  }

  Serial.println("I2S 总线已初始化。");
}

void Mic::recordWav(int recordTime, MicCallback callback)
{
  // 创建用于存储音频数据的变量
  uint8_t *wav_buffer;
  size_t wav_size;
  // 录制 20 秒的音频数据
  wav_buffer = i2s.recordWAV(recordTime, &wav_size);
  Serial.print("record wav_size=");
  Serial.println(wav_size);
  
  callback(wav_buffer, wav_size);
}

size_t Mic::read(char *buffer, size_t size) {
  return i2s.readBytes(buffer, size);
}

void Mic::loop() {
//   size_t bytesRead = 0;
//   esp_err_t result = i2s_read(I2S_PORT, sBuffer, bufferLen * sizeof(int16_t), &bytesRead, portMAX_DELAY);
//   if (result == ESP_OK && bytesRead > 0) {
//     // 在这里可以对读取到的音频数据进行处理，比如保存到文件、发送到其他设备等
//     // 目前只是简单地打印一些信息
//     Serial.print("Bytes Read: ");
//     Serial.println(bytesRead);
//   }
}