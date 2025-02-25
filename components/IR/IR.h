#ifndef _IR_H_
#define _IR_H_
#include <Arduino.h>
#include <assert.h>
#include "ir_learn.h"
#include "ir_encoder.h"
#include <vector>
using namespace std;

#define IR_TX_GPIO_NUM                  GPIO_NUM_39
#define IR_RX_GPIO_NUM                  GPIO_NUM_38
#define IR_RESOLUTION_HZ                1000000 // 1MHz resolution, 1 tick = 1us

#define IR_BUFFER_SIZE 1024

class Ir {
    public:
        Ir();
        ~Ir();
        static void delegate(void *pvParameters) {
          Ir* instance = static_cast<Ir*>(pvParameters);
          while(1) {
            instance->loop();
            delay(1);
          }
        }

        void setup() {

        }
        void startLearn();
        void stopLearn();
        void send(rmt_rx_done_event_data_t data);

    private:
      void loop();
      void initRXChannel();
      void initTXChannel();
      // void ir_learn_auto_learn_cb(ir_learn_state_t state, unsigned char sub_step, struct ir_learn_sub_list_head *data);
      // void ir_learn_save_result(struct ir_learn_sub_list_head *data_save, struct ir_learn_sub_list_head *data_src);
      rmt_channel_handle_t rx_channel;
      rmt_channel_handle_t tx_channel;

      QueueHandle_t mReceive_queue;
      vector<rmt_rx_done_event_data_t> mIRDataList;
};

#endif