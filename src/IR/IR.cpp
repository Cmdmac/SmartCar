#include "IR.h"
/* FreeRTOS includes */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "driver/rmt_types.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_rx.h"
#include "src/ir/include/ir_encoder.h"

ir_learn_handle_t handle = NULL;
struct ir_learn_sub_list_head ir_results;    /**< IR learn test result */
extern char* TAG;

static void ir_learn_save_result(struct ir_learn_sub_list_head *data_save, struct ir_learn_sub_list_head *data_src)
{
    assert(data_src && "rmt_symbols is null");

    struct ir_learn_sub_list_t *sub_it;
    struct ir_learn_sub_list_t *last;

    last = SLIST_FIRST(data_src);
    while ((sub_it = SLIST_NEXT(last, next)) != NULL) {
        last = sub_it;
    }
    ir_learn_add_sub_list_node(data_save, last->timediff, &last->symbols);

    return;
}

 void ir_learn_auto_learn_cb(ir_learn_state_t state, uint8_t sub_step, struct ir_learn_sub_list_head *data)
 {
   switch (state) {
   /**< IR learn ready, after successful initialization */
   case IR_LEARN_STATE_READY:
      ESP_LOGI(TAG, "IR Learn ready");
      break;
   /**< IR learn exit */
   case IR_LEARN_STATE_EXIT:
      ESP_LOGI(TAG, "IR Learn exit");
      break;
   /**< IR learn successfully */
   case IR_LEARN_STATE_END:
      ESP_LOGI(TAG, "IR Learn end");
      ir_learn_save_result(&ir_results, data);
      ir_learn_print_raw(data);
      ir_learn_stop(&handle);
      break;
   /**< IR learn failure */
   case IR_LEARN_STATE_FAIL:
      ESP_LOGI(TAG, "IR Learn failed, retry");
      break;
   /**< IR learn step, start from 1 */
   case IR_LEARN_STATE_STEP:
   default:
      ESP_LOGI(TAG, "IR Learn step:[%d][%d]", state, sub_step);
      break;
   }
   return;
}

static void ir_learn_test_tx_raw(struct ir_learn_sub_list_head *rmt_out)
{
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


    rmt_tx_channel_config_t tx_channel_cfg = {
        .gpio_num = IR_TX_GPIO_NUM,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = IR_RESOLUTION_HZ,
        .mem_block_symbols = 128,
        .trans_queue_depth = 4,

    };
    rmt_channel_handle_t tx_channel = NULL;
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_channel_cfg, &tx_channel));

    rmt_carrier_config_t carrier_cfg = {
        .frequency_hz = 38000, // 38KHz
        .duty_cycle = 0.33,
    };
//     typedef struct {
//     uint32_t frequency_hz; /*!< Carrier wave frequency, in Hz, 0 means disabling the carrier */
//     float duty_cycle;      /*!< Carrier wave duty cycle (0~100%) */
//     struct {
//         uint32_t polarity_active_low: 1; /*!< Specify the polarity of carrier, by default it's modulated to base signal's high level */
//         uint32_t always_on: 1;           /*!< If set, the carrier can always exist even there's not transfer undergoing */
//     } flags;                             /*!< Carrier config flags */
// } rmt_carrier_config_t;
    ESP_ERROR_CHECK(rmt_apply_carrier(tx_channel, &carrier_cfg));

    rmt_transmit_config_t transmit_cfg = {
        .loop_count = 0, // no loop
    };

    ir_encoder_config_t raw_encoder_cfg = {
        .resolution = IR_RESOLUTION_HZ,
    };
    rmt_encoder_handle_t raw_encoder = NULL;
    ESP_ERROR_CHECK(ir_encoder_new(&raw_encoder_cfg, &raw_encoder));

    ESP_ERROR_CHECK(rmt_enable(tx_channel));

    struct ir_learn_sub_list_t *sub_it;
    SLIST_FOREACH(sub_it, rmt_out, next) {
        vTaskDelay(pdMS_TO_TICKS(sub_it->timediff / 1000));

        rmt_symbol_word_t *rmt_symbols = sub_it->symbols.received_symbols;
        size_t symbol_num = sub_it->symbols.num_symbols;

        ESP_ERROR_CHECK(rmt_transmit(tx_channel, raw_encoder, rmt_symbols, symbol_num, &transmit_cfg));
        rmt_tx_wait_all_done(tx_channel, -1);
    }

    rmt_disable(tx_channel);
    rmt_del_channel(tx_channel);
    raw_encoder->del(raw_encoder);
}

void IR::startLearn(int learnCount) {
    const ir_learn_cfg_t config = {
    .clk_src = RMT_CLK_SRC_DEFAULT,/*!< RMT clock source */
    .resolution = 1000000,         /*!< RMT resolution, 1M, 1 tick = 1us*/

    .learn_count = learnCount,              /*!< IR learn count needed */
    .learn_gpio = 10,     /*!< IR learn io that consumed by the sensor */
    .callback = ir_learn_auto_learn_cb,       /*!< IR learn result callback for user */

    .task_priority = 5,   /*!< IR learn task priority */
    .task_stack = 4096,   /*!< IR learn task stack size */
    .task_affinity = 1,   /*!< IR learn task pinned to core (-1 is no affinity) */
  };

  ESP_ERROR_CHECK(ir_learn_new(&config, &handle));
}
    
void IR::stopLearn() {
    ir_learn_stop(&handle);
}

void IR::send(struct ir_learn_sub_list_head *rmt_out) {
    ir_learn_test_tx_raw(rmt_out);
}