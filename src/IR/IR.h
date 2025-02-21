#ifndef _IR_H_
#define _IR_H_
#include <Arduino.h>
#include <assert.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>
/* FreeRTOS includes */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define IR_TX_GPIO_NUM                  GPIO_NUM_39
#define IR_RX_GPIO_NUM                  GPIO_NUM_38


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
      // Use turn on the save buffer feature for more complete capture coverage.
      IRrecv mIRRecv;//(kRecvPin, kCaptureBufferSize, kTimeout, true);
      decode_results mDecodeResults;// results;  // Somewhere to store the results
      TaskHandle_t mTaskHandle;
};

#endif