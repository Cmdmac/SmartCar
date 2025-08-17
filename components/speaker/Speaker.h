#ifndef _SPEAKER_H
#define _SPEAKER_H
#include <Arduino.h>

#include <AudioTools.h>
#include "AudioTools/Disk/AudioSourceURL.h"
#include <AudioTools/AudioCodecs/CodecMP3Helix.h>  // Helix MP3解码器
// #include <WiFiClientSecure.h>
// #include <libmad.h>
class Speaker {

    private:
        int bclk, lrc, dout;
        // Audio audio;
        static void playDelegate(void *pvParameters) {
          Speaker* instance = static_cast<Speaker*>(pvParameters);
          AudioSourceURL audioSource = AudioSourceDynamicURL(instance->urlStream, "audio/mp3");
          AudioPlayer audioPlayer(audioSource, instance->audioOutput, instance->decoder);
          while(1) {
            // instance->loop();
            // Serial.println("Speaker loop running");
            audioPlayer.begin();

            delay(1);
          }
        }
        void loop();

        static const char* TAG;
        static const size_t AUDIO_BUFFER_SIZE = 8192;
        static const size_t OUTPUT_BUFFER_SIZE = 4096;
        
        // WiFiClientSecure client;       // 安全网络客户端
        URLStream urlStream;           // 音频流
        I2SStream i2s;                 // I2S输出
        MP3DecoderHelix decoder;       // Helix MP3解码器
        EncodedAudioStream audioOutput;// 解码后音频流

        // StreamCopy copier;  // 用于流复制
        bool is_playing;
    public:
        Speaker(int bclk, int lrc, int dout) : bclk(bclk), lrc(lrc), dout(dout),
          audioOutput(&i2s, &decoder),
          // copier(audioOutput, urlStream),  // 初始化流复制器
          is_playing(false) {

        }
        void setup();
        void play(const char* source);
        void stop();
        void setVolume(float volume);

};

#endif