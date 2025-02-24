#ifndef _IR_H_
#define _IR_H_
#include <Arduino.h>
#include <assert.h>
#include "ir_learn.h"
#include "ir_encoder.h"

#define IR_TX_GPIO_NUM                  GPIO_NUM_39
#define IR_RX_GPIO_NUM                  GPIO_NUM_38
#define IR_RESOLUTION_HZ                1000000 // 1MHz resolution, 1 tick = 1us

#define IR_BUFFER_SIZE 1024

class IR {
    public:
        IR();
        static void delegate(void *pvParameters) {
          IR* instance = static_cast<IR*>(pvParameters);
          while(1) {
            instance->loop();
            delay(1);
          }
        }

        void setup() {

        }
        void startLearn();
        void stopLearn();
        void send();
    private:
      void loop();
      // void ir_learn_auto_learn_cb(ir_learn_state_t state, unsigned char sub_step, struct ir_learn_sub_list_head *data);
      // void ir_learn_save_result(struct ir_learn_sub_list_head *data_save, struct ir_learn_sub_list_head *data_src);

};

#endif