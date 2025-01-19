#include "Mic.h"
// #include "net.h"
// #include <driver/i2s.h>
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
void Mic::setup(int pinWS, int pinSD, int pinSCK) {
    Serial.println("AudioRecorder Setup I2S...");
    /*
    esp_err_t err;

    // 配置 I2S
    i2s_chan_config_t i2s_chan_config = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM, I2S_ROLE_MASTER, I2S_MODE_RX);
    i2s_chan_config.dma_desc_num = 6;
    i2s_chan_config.dma_frame_num = 128;
    i2s_chan_config.auto_clear = false;
    i2s_chan_config.sample_rate_hz = SAMPLE_RATE;
    i2s_chan_config.bits_per_sample = BITS_PER_SAMPLE;
    i2s_chan_config.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
    i2s_chan_config.communication_format = I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB;

    // 配置 I2S 引脚
    i2s_std_config_t i2s_std_config = I2S_STD_CONFIG_DEFAULT(I2S_NUM, BITS_PER_SAMPLE, I2S_CHANNEL);
    i2s_std_config.bclk_io_num = 26;
    i2s_std_config.ws_io_num = 25;
    i2s_std_config.data_in_io_num = 33;
    i2s_std_config.data_out_io_num = I2S_PIN_NO_CHANGE;

    // 安装 I2S 通道驱动
    err = i2s_new_channel(&i2s_chan_config, &i2s_event_handler, NULL);
    if (err!= ESP_OK) {
        ESP_LOGE(TAG, "Error installing I2S channel driver: %d", err);
        return;
    }

    // 配置 I2S 标准接口
    err = i2s_channel_init_std_mode(I2S_NUM, &i2s_std_config);
    if (err!= ESP_OK) {
        ESP_LOGE(TAG, "Error initializing I2S standard mode: %d", err);
        return;
    }

    // // 初始化 SD 卡
    // err = init_sd_card();
    // if (err!= ESP_OK) {
    //     ESP_LOGE(TAG, "Error initializing SD card: %d", err);
    //     return;
    // }

    // 启动 I2S 接收
    err = i2s_channel_enable(I2S_NUM);
    if (err!= ESP_OK) {
        ESP_LOGE(TAG, "Error enabling I2S: %d", err);
        return;
    }
    */
}

void Mic::recordWav(const char *url, int recordTime, int sampleRate, int sampleBits)
{
  /*
  size_t sample_size = 0;
  uint32_t record_size = (SAMPLE_RATE * SAMPLE_BITS / 8) * RECORD_TIME;
  uint8_t *rec_buffer = NULL;
  Serial.printf("Start recording ...\n");
   
  // HTTPClient http;
  // NetHttpFile file = NetHttpFile(url, "test.wav"); 
  File file = SD.open(url, FILE_WRITE);
  // Write the header to the WAV file
  uint8_t wav_header[WAV_HEADER_SIZE];
  generateWavHeader(wav_header, record_size, SAMPLE_RATE);
  file.write(wav_header, WAV_HEADER_SIZE);

  // PSRAM malloc for recording
  rec_buffer = (uint8_t *)ps_malloc(record_size);
  if (rec_buffer == NULL) {
    Serial.printf("malloc failed!\n");
    while(1) ;
  }
  Serial.printf("Buffer: %d bytes\n", ESP.getPsramSize() - ESP.getFreePsram());

  // Start recording
  i2s_read(I2S_IN_PORT, rec_buffer, record_size, &sample_size, portMAX_DELAY);
  if (sample_size == 0) {
    Serial.printf("Record Failed!\n");
  } else {
    Serial.printf("Record %d bytes\n", sample_size);
  }

  // Increase volume
  for (uint32_t i = 0; i < sample_size; i += SAMPLE_BITS/8) {
    (*(uint16_t *)(rec_buffer+i)) <<= VOLUME_GAIN;
  }

  // Write data to the WAV file
  Serial.printf("Writing to the file ...\n");
  if (file.write(rec_buffer, record_size) != record_size)
    Serial.printf("Write file Failed!\n");

  free(rec_buffer);
  file.close();
  Serial.printf("Recording complete: \n");
  Serial.printf("Send rec for a new sample or enter a new label\n\n");
  */
}

void Mic::generateWavHeader(uint8_t *wav_header, uint32_t wav_size, uint32_t sample_rate)
{
  // See this for reference: http://soundfile.sapp.org/doc/WaveFormat/
  uint32_t file_size = wav_size + WAV_HEADER_SIZE - 8;
  uint32_t byte_rate = SAMPLE_RATE * SAMPLE_BITS / 8;
  const uint8_t set_wav_header[] = {
    'R', 'I', 'F', 'F', // ChunkID
    file_size, file_size >> 8, file_size >> 16, file_size >> 24, // ChunkSize
    'W', 'A', 'V', 'E', // Format
    'f', 'm', 't', ' ', // Subchunk1ID
    0x10, 0x00, 0x00, 0x00, // Subchunk1Size (16 for PCM)
    0x01, 0x00, // AudioFormat (1 for PCM)
    0x01, 0x00, // NumChannels (1 channel)
    sample_rate, sample_rate >> 8, sample_rate >> 16, sample_rate >> 24, // SampleRate
    byte_rate, byte_rate >> 8, byte_rate >> 16, byte_rate >> 24, // ByteRate
    0x02, 0x00, // BlockAlign
    0x10, 0x00, // BitsPerSample (16 bits)
    'd', 'a', 't', 'a', // Subchunk2ID
    wav_size, wav_size >> 8, wav_size >> 16, wav_size >> 24, // Subchunk2Size
  };
  memcpy(wav_header, set_wav_header, sizeof(set_wav_header));
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