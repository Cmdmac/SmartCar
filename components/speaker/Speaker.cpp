#include "Speaker.h"

// #define I2S_DOUT      15
// #define I2S_BCLK      16
// #define I2S_LRC       17

void Speaker::setup() {
    pinMode(47, OUTPUT);
    digitalWrite(47, HIGH);
    // 配置I2S - 根据实际硬件修改



    // 配置安全客户端（跳过证书验证）
    // client.setInsecure();

    // audio.setPinout(bclk, lrc, dout);
    // audio.setVolume(18);
    // xTaskCreatePinnedToCore(&Speaker::playDelegate, "SpeakerTask", 4096 * 2, this, 1, NULL, 0);
}

void Speaker::play(const char* source) {
    if (is_playing) stop();
    
    Serial.printf("Playing: %s\n", source);
    is_playing = true;
    

    
    // // 重置复制器
    // copier.resize
    
    // 开始播放
    // while (urlStream && is_playing) {
    //     Serial.printf("Playing in while");

    //   copier.copy();  // 复制数据到解码器
    //   delay(1);       // 防止看门狗复位
    // }
    // Serial.printf("end while");

    // stop();
    xTaskCreatePinnedToCore(&Speaker::playDelegate, "SpeakerTask", 4096 * 2, this, 1, NULL, 0);

}

void Speaker::stop() {
    if (!is_playing) return;
    
    is_playing = false;

    Serial.println("Playback stopped");
}

void Speaker::setVolume(float volume) {
    // 设置音量 (0.0 ~ 1.0)
    // i2s.setVolume(volume);
}

void Speaker::loop() {
    // audio.loop();
}

const char* Speaker::TAG = "Speaker";
