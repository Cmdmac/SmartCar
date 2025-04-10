#ifndef _IR_H_
#define _IR_H_
#include <Arduino.h>
#include <assert.h>
#include "ir_learn.h"
#include "ir_encoder.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_rx.h"
#include <vector>
using namespace std;


#define IR_RESOLUTION_HZ                1000000 // 1MHz resolution, 1 tick = 1us

#define IR_BUFFER_SIZE 1024

#define EXAMPLE_IR_NEC_DECODE_MARGIN 200     // Tolerance for parsing RMT symbols into bit stream

/**
 * @brief NEC timing spec
 */
#define NEC_LEADING_CODE_DURATION_0  9000
#define NEC_LEADING_CODE_DURATION_1  4500
#define NEC_PAYLOAD_ZERO_DURATION_0  560
#define NEC_PAYLOAD_ZERO_DURATION_1  560
#define NEC_PAYLOAD_ONE_DURATION_0   560
#define NEC_PAYLOAD_ONE_DURATION_1   1690
#define NEC_REPEAT_CODE_DURATION_0   9000
#define NEC_REPEAT_CODE_DURATION_1   2250

struct NecCode
{
    uint16_t address;
    uint16_t command;
};

class Ir {
    public:
      Ir(gpio_num_t txPin, gpio_num_t rxPin) : txPin(txPin), rxPin(rxPin), tx_channel(NULL) {

      }
      ~Ir();
      static void delegate(void *pvParameters) {
        Ir* instance = static_cast<Ir*>(pvParameters);
        // while(1) {
          instance->loop();
        //   delay(1);
        // }
      }

      void setup() {
        mIrDatalistMux = xSemaphoreCreateMutex();
      }
      void startLearn();
      void stopLearn();
      void send(NecCode data);

      void initIrDatas() {
        // uint32_t list[34] = {2441814799,2183332448,2183332420,2184839726,2183332421,2184774190,2183332421,2183463491,2257322542,2255815262,
        //                     2257257030,2255618658,2257388102,2257257032,2257257031,2257125963,2184774217,2257191470,2184839753,2184839726,
        //                     2184905261,2184774190,2184905262,2184839725,2184839726,2183397956,2255618629,2255684192,2257257033,2255684192,2257322568,2255749727,2255684192,2147484233};

        // rmt_rx_done_event_data_t data;
        // rmt_symbol_word_t symbols[34];
        // data.received_symbols = symbols;
        // for (size_t i = 0; i < 34; i++)
        // {
        //   data.received_symbols[i].val = list[i];
        // }
        
        // data.num_symbols = 34;
        delay(2 * 1000);
        Serial.println("send");
        NecCode code = {
          .address = 0x7F80,
          .command = 0xFE01,
        };
        send(code);
        // synmbol.num_symbols = sizeof(synmbol.received_symbols);
      }

      void printIrData(rmt_rx_done_event_data_t rx_data) {
        for (int j = 0; j < rx_data.num_symbols; j++) {
          rmt_symbol_word_t received_symbols = rx_data.received_symbols[j];
          printf("%lu,", received_symbols.val);
        }
        printf("\r\n");
      }

      void printIrDatalist() {
        // if (xSemaphoreTake(mIrDatalistMux, portMAX_DELAY) == pdTRUE) {
        //   for (size_t i = 0; i < mIrDataList.size(); i++)
        //   {
        //     rmt_rx_done_event_data_t data = mIrDataList[i];
        //     printIrData(data);
        //   }          
        //    // 释放互斥锁
        //   xSemaphoreGive(mIrDatalistMux);
        // }
      }
    private:
      void loop();
      void initRXChannel();
      void initTXChannel();
      void addIrData(NecCode data) {
        // 获取互斥锁
        if (xSemaphoreTake(mIrDatalistMux, portMAX_DELAY) == pdTRUE) {
          mCodes.push_back(data);
           // 释放互斥锁
          xSemaphoreGive(mIrDatalistMux);
        }
      }
      /**
       * @brief Check whether a duration is within expected range
       */
      static inline bool nec_check_in_range(uint32_t signal_duration, uint32_t spec_duration)
      {
          return (signal_duration < (spec_duration + EXAMPLE_IR_NEC_DECODE_MARGIN)) &&
                (signal_duration > (spec_duration - EXAMPLE_IR_NEC_DECODE_MARGIN));
      }

      /**
       * @brief Check whether a RMT symbol represents NEC logic zero
       */
      static bool nec_parse_logic0(rmt_symbol_word_t *rmt_nec_symbols)
      {
          return nec_check_in_range(rmt_nec_symbols->duration0, NEC_PAYLOAD_ZERO_DURATION_0) &&
                nec_check_in_range(rmt_nec_symbols->duration1, NEC_PAYLOAD_ZERO_DURATION_1);
      }

      /**
       * @brief Check whether a RMT symbol represents NEC logic one
       */
      static bool nec_parse_logic1(rmt_symbol_word_t *rmt_nec_symbols)
      {
          return nec_check_in_range(rmt_nec_symbols->duration0, NEC_PAYLOAD_ONE_DURATION_0) &&
                nec_check_in_range(rmt_nec_symbols->duration1, NEC_PAYLOAD_ONE_DURATION_1);
      }

      bool nec_parse_frame_repeat(rmt_symbol_word_t *rmt_nec_symbols);
      bool nec_parse_frame(rmt_symbol_word_t *rmt_nec_symbols, NecCode* code);
      void parse_nec_frame(rmt_symbol_word_t *rmt_nec_symbols, size_t symbol_num);
      // void ir_learn_auto_learn_cb(ir_learn_state_t state, unsigned char sub_step, struct ir_learn_sub_list_head *data);
      // void ir_learn_save_result(struct ir_learn_sub_list_head *data_save, struct ir_learn_sub_list_head *data_src);
      gpio_num_t txPin, rxPin;
      // rmt_channel_handle_t rx_channel;
      rmt_channel_handle_t tx_channel;

      SemaphoreHandle_t mIrDatalistMux;
      // vector<rmt_rx_done_event_data_t> mIrDataList;
      vector<NecCode> mCodes;
      // uint16_t s_nec_code_address;
      // uint16_t s_nec_code_command;

};

#endif