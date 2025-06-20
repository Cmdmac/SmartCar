#include "Ir.h"
/* FreeRTOS includes */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#include "driver/gpio.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_rx.h"
#include "ir_nec_encoder.h"


static const char* TAG = "IR";

static bool example_rmt_rx_done_callback(rmt_channel_handle_t channel, const rmt_rx_done_event_data_t *edata, void *user_data)
{
    BaseType_t high_task_wakeup = pdFALSE;
    QueueHandle_t receive_queue = (QueueHandle_t)user_data;
    // send the received RMT symbols to the parser task
    xQueueSendFromISR(receive_queue, edata, &high_task_wakeup);
    return high_task_wakeup == pdTRUE;
}

Ir::~Ir() {
  if (tx_channel != NULL) {
    rmt_disable(tx_channel);
    rmt_del_channel(tx_channel);
  }
}

void Ir::initTXChannel() {
 
}

void Ir::initRXChannel() {
  
}

bool Ir::nec_parse_frame_repeat(rmt_symbol_word_t *rmt_nec_symbols)
{
    return nec_check_in_range(rmt_nec_symbols->duration0, NEC_REPEAT_CODE_DURATION_0) &&
           nec_check_in_range(rmt_nec_symbols->duration1, NEC_REPEAT_CODE_DURATION_1);
}

/**
 * @brief Decode RMT symbols into NEC scan code and print the result
 */
void Ir::parse_nec_frame(rmt_symbol_word_t *rmt_nec_symbols, size_t symbol_num)
{
  /*
    printf("NEC frame start---\r\n");
    for (size_t i = 0; i < symbol_num; i++) {
        printf("{%d:%d},{%d:%d}\r\n", rmt_nec_symbols[i].level0, rmt_nec_symbols[i].duration0,
               rmt_nec_symbols[i].level1, rmt_nec_symbols[i].duration1);
    }
    printf("---NEC frame end: ");
  */
  // decode RMT symbols
  NecCode code;
  code.address = -1;
  code.command = -1;
  switch (symbol_num) {
  case 34: // NEC normal frame

      if (nec_parse_frame(rmt_nec_symbols, &code)) {
          printf("Address=%04X, Command=%04X\r\n\r\n", code.address, code.command);
      }
      mCodes.push_back(code);
      break;
  case 2: // NEC repeat frame
      if (nec_parse_frame_repeat(rmt_nec_symbols)) {
          printf("Address=%04X, Command=%04X, repeat\r\n\r\n", code.address, code.command);
      }
      break;
  default:
      printf("Unknown NEC frame %d\r\n\r\n", symbol_num);
      break;
  }
}

bool Ir::nec_parse_frame(rmt_symbol_word_t *rmt_nec_symbols, NecCode *code)
{
    rmt_symbol_word_t *cur = rmt_nec_symbols;
    uint16_t address = 0;
    uint16_t command = 0;
    bool valid_leading_code = nec_check_in_range(cur->duration0, NEC_LEADING_CODE_DURATION_0) &&
                              nec_check_in_range(cur->duration1, NEC_LEADING_CODE_DURATION_1);
    if (!valid_leading_code) {
        return false;
    }
    cur++;
    for (int i = 0; i < 16; i++) {
        if (nec_parse_logic1(cur)) {
            address |= 1 << i;
        } else if (nec_parse_logic0(cur)) {
            address &= ~(1 << i);
        } else {
            return false;
        }
        cur++;
    }
    for (int i = 0; i < 16; i++) {
        if (nec_parse_logic1(cur)) {
            command |= 1 << i;
        } else if (nec_parse_logic0(cur)) {
            command &= ~(1 << i);
        } else {
            return false;
        }
        cur++;
    }
    // save address and command
    // s_nec_code_address = address;
    // s_nec_code_command = command;
    // NecCode code;
    code->address = address;
    code->command = command;
    return true;
}

void Ir::startLearn() {
//   typedef struct {
//     rmt_clock_source_t clk_src; /*!< RMT clock source */
//     uint32_t resolution;        /*!< RMT resolution, in Hz */

//     int learn_count;            /*!< IR learn count needed */
//     int learn_gpio;             /*!< IR learn io that consumed by the sensor */
//     ir_learn_result_cb callback;/*!< IR learn result callback for user */

//     int task_priority;          /*!< IR learn task priority */
//     int task_stack;             /*!< IR learn task stack size */
//     int task_affinity;          /*!< IR learn task pinned to core (-1 is no affinity) */
// } ir_learn_cfg_t;
  // if (rx_channel == NULL) {
  //   initRXChannel();
  // }
  //运行在当前Core
  xTaskCreatePinnedToCore(&Ir::delegate, "IRLearnTask", 4096, this, 1, NULL, xPortGetCoreID());
}
    
void Ir::stopLearn() {

}

void Ir::send(NecCode data) {
  // void ir_learn_test_tx_raw(struct ir_learn_sub_list_head *rmt_out)
  // {
//   typedef struct {
//     gpio_num_t gpio_num;        /*!< GPIO number used by RMT TX channel. Set to -1 if unused */
//     rmt_clock_source_t clk_src; /*!< Clock source of RMT TX channel, channels in the same group must use the same clock source */
//     uint32_t resolution_hz;     /*!< Channel clock resolution, in Hz */
//     size_t mem_block_symbols;   /*!< Size of memory block, in number of `rmt_symbol_word_t`, must be an even.
//                                      In the DMA mode, this field controls the DMA buffer size, it can be set to a large value;
//                                      In the normal mode, this field controls the number of RMT memory block that will be used by the channel. */
//     size_t trans_queue_depth;   /*!< Depth of internal transfer queue, increase this value can support more transfers pending in the background */
//     int intr_priority;          /*!< RMT interrupt priority,
//                                      if set to 0, the driver will try to allocate an interrupt with a relative low priority (1,2,3) */
//     struct {
//         uint32_t invert_out: 1;   /*!< Whether to invert the RMT channel signal before output to GPIO pad */
//         uint32_t with_dma: 1;     /*!< If set, the driver will allocate an RMT channel with DMA capability */
//         uint32_t io_loop_back: 1; /*!< The signal output from the GPIO will be fed to the input path as well */
//         uint32_t io_od_mode: 1;   /*!< Configure the GPIO as open-drain mode */
//     } flags;                      /*!< TX channel config flags */
// } rmt_tx_channel_config_t;

    
  // }
  // rmt_symbol_word_t *rmt_symbols = data.received_symbols;
  // size_t symbol_num = data.num_symbols;
  if (tx_channel == NULL) {
    ESP_LOGI(TAG, "create RMT TX channel");
    rmt_tx_channel_config_t tx_channel_cfg = {
        .gpio_num = txPin,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = IR_RESOLUTION_HZ,
        .mem_block_symbols = 64, // amount of RMT symbols that the channel can store at a time
        .trans_queue_depth = 4,  // number of transactions that allowed to pending in the background, this example won't queue multiple transactions, so queue depth > 1 is sufficient
    };
    // rmt_channel_handle_t tx_channel = NULL;
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_channel_cfg, &tx_channel));

    ESP_LOGI(TAG, "modulate carrier to TX channel");
    rmt_carrier_config_t carrier_cfg = {
        .frequency_hz = 38000, // 38KHz
        .duty_cycle = 0.33,
    };
    ESP_ERROR_CHECK(rmt_apply_carrier(tx_channel, &carrier_cfg));

    ESP_LOGI(TAG, "enable RMT TX channels");
    ESP_ERROR_CHECK(rmt_enable(tx_channel));
  }
  
  if (nec_encoder == NULL) {
        ESP_LOGI(TAG, "install IR NEC encoder");
        ir_nec_encoder_config_t nec_encoder_cfg = {
            .resolution = IR_RESOLUTION_HZ,
        };
        // rmt_encoder_handle_t nec_encoder = NULL;
        ESP_ERROR_CHECK(rmt_new_ir_nec_encoder(&nec_encoder_cfg, &nec_encoder));
  }

  // Address=7F80, Command=FE01

  const ir_nec_scan_code_t scan_code = {
      .address = data.address,
      .command = data.command,
  };
    // this example won't send NEC frames in a loop
  rmt_transmit_config_t transmit_config = {
      .loop_count = 0, // no loop
  };
  ESP_ERROR_CHECK(rmt_transmit(tx_channel, nec_encoder, &scan_code, sizeof(scan_code), &transmit_config));
  rmt_tx_wait_all_done(tx_channel, -1);

  Serial.println("send end");
  // ESP_ERROR_CHECK(rmt_transmit(tx_channel, nec_encoder, &scan_code, sizeof(scan_code), &transmit_config));

}

void Ir::loop() {
  ESP_LOGI(TAG, "create RMT RX channel");
  rmt_rx_channel_config_t rx_channel_cfg = {
      .gpio_num = rxPin,
      .clk_src = RMT_CLK_SRC_DEFAULT,
      .resolution_hz = IR_RESOLUTION_HZ,
      .mem_block_symbols = 64, // amount of RMT symbols that the channel can store at a time
  };
  rmt_channel_handle_t rx_channel = NULL;
  ESP_ERROR_CHECK(rmt_new_rx_channel(&rx_channel_cfg, &rx_channel));

  ESP_LOGI(TAG, "register RX done callback");
  QueueHandle_t receive_queue = xQueueCreate(1, sizeof(rmt_rx_done_event_data_t));
  assert(receive_queue);
  rmt_rx_event_callbacks_t cbs = {
      .on_recv_done = example_rmt_rx_done_callback,
  };
  ESP_ERROR_CHECK(rmt_rx_register_event_callbacks(rx_channel, &cbs, receive_queue));

  ESP_LOGI(TAG, "enable RMT RX channel");
  ESP_ERROR_CHECK(rmt_enable(rx_channel));

  // the following timing requirement is based on NEC protocol
  rmt_receive_config_t receive_config = {
      .signal_range_min_ns = 1250,     // the shortest duration for NEC signal is 560us, 1250ns < 560us, valid signal won't be treated as noise
      .signal_range_max_ns = 12000000, // the longest duration for NEC signal is 9000us, 12000000ns > 9000us, the receive won't stop early
  };
  // save the received RMT symbols
  rmt_symbol_word_t raw_symbols[64]; // 64 symbols should be sufficient for a standard NEC frame
  rmt_rx_done_event_data_t rx_data;
  // ready to receive
  ESP_ERROR_CHECK(rmt_receive(rx_channel, raw_symbols, sizeof(raw_symbols), &receive_config));
  while (1) {
    // wait for RX done signal
    if (xQueueReceive(receive_queue, &rx_data, pdMS_TO_TICKS(1000)) == pdPASS) {
        // parse the receive symbols and print the result
        // example_parse_nec_frame(rx_data.received_symbols, rx_data.num_symbols);
        parse_nec_frame(rx_data.received_symbols, rx_data.num_symbols);
        // printIrData(rx_data);
        // addIrData(rx_data);
        // send(rx_data);
        // printIrDatalist();
        // start receive again
        ESP_ERROR_CHECK(rmt_receive(rx_channel, raw_symbols, sizeof(raw_symbols), &receive_config));
    } else {
        // timeout, transmit predefined IR NEC packets
        // const ir_nec_scan_code_t scan_code = {
        //     .address = 0x0440,
        //     .command = 0x3003,
        // };
        // ESP_ERROR_CHECK(rmt_transmit(tx_channel, nec_encoder, &scan_code, sizeof(scan_code), &transmit_config));
    }
  }
  rmt_disable(rx_channel);
  rmt_del_channel(rx_channel);
}