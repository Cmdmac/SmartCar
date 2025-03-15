#pragma once
#include "esp_afe_sr_iface.h"
#include "esp_afe_sr_models.h"
#include "esp_wn_iface.h"
#include "model_path.h"
// #include "esp_wn_models.h
#include "esp_mn_iface.h"
#include "esp_mn_models.h"
#include "esp_mn_speech_commands.h"
// #include "Mic.h"
// #include "driver/i2s_std.h"

#define ADC_I2S_CHANNEL 2

class Sr {
    public:
        void setup();
    
    private:
      void init_i2s();
      int get_feed_channel(void);
      esp_err_t get_feed_data(bool is_get_raw_channel, int16_t *buffer, int buffer_len);

      void detect_Task(void *arg);
      static void detectDelegate(void *pvParameters) {
        Sr* instance = static_cast<Sr*>(pvParameters);
        instance->detect_Task(instance->afe_data);
      }

      void feed_Task(void *arg);
      static void feedDelegate(void *pvParameters) {
        Sr* instance = static_cast<Sr*>(pvParameters);
        instance->feed_Task(instance->afe_data);
      }
      srmodel_list_t *models = NULL;
      esp_afe_sr_iface_t *afe_handle = NULL;
      esp_afe_sr_data_t *afe_data = NULL;

      int detect_flag = 0;
      volatile int task_flag = 0;

      // i2s_chan_handle_t rx_handle;

      // Mic mic;
};