
#include "Sr.h"
#include "esp_log.h"

// #include "esp32_s3_szp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_process_sdkconfig.h"
#include "esp_codec_dev.h"
#include "esp_codec_dev_defaults.h"
#include "esp_afe_config.h"
// #include "audio_player.h"
// #include "app_ui.h"

static const char *TAG = "app_sr";
// #include "driver/i2s.h"
#define MAX98375_BCLK_IO1 5
#define MAX98375_LRCLK_IO1 4
#define MAX98375_DOUT_IO1 6
// const char* host = "192.168.1.4"; // 电脑的IP地址
// const int port = 8888; // 监听的端口
#define bufferLen 512

#include "WiFiClient.h"

// void Init_i2s(){
//     i2s_config_t i2s_config = {
//         .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
//         .sample_rate = 16000,
//         .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
//         .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
//         .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
//         .intr_alloc_flags = 0,
//         .dma_buf_count = 8,
//         .dma_buf_len = bufferLen,
//         .use_apll = false          // 分配中断标志
//     };
//     ESP_ERROR_CHECK(i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL));
//     i2s_pin_config_t pin_config = {
//         .bck_io_num = MAX98375_BCLK_IO1,            // BCLK引脚号
//         .ws_io_num = MAX98375_LRCLK_IO1,             // LRCK引脚号
//         .data_out_num = -1, // DATA引脚号
//         .data_in_num = MAX98375_DOUT_IO1,           // DATA_IN引脚号
//     };
//     ESP_ERROR_CHECK(i2s_set_pin(I2S_NUM_0, &pin_config));
//   //   ESP_ERROR_CHECK(i2s_start(I2S_NUM_0));
    
//   }

int Sr::get_feed_channel(void)
{
    return ADC_I2S_CHANNEL;
}

esp_err_t Sr::get_feed_data(bool is_get_raw_channel, int16_t *buffer, int buffer_len)
{
    esp_err_t ret = ESP_OK;
    
    int audio_chunksize = buffer_len / (sizeof(int16_t) * ADC_I2S_CHANNEL);
    // ret = esp_codec_dev_read(record_dev_handle, (void *)buffer, buffer_len);
    // size_t len = mic.read(reinterpret_cast<char*>(buffer), buffer_len);
    // ESP_LOGI(TAG, "mic read len=%d", len);
    if (!is_get_raw_channel) {
        for (int i = 0; i < audio_chunksize; i++) {
            int16_t ref = buffer[4 * i + 0];
            buffer[3 * i + 0] = buffer[4 * i + 1];
            buffer[3 * i + 1] = buffer[4 * i + 3];
            buffer[3 * i + 2] = ref;
        }
    }

    return ret;
}

void Sr::feed_Task(void *arg)
{
    esp_afe_sr_data_t *afe_data = (esp_afe_sr_data_t*)arg;  // 获取参数
    int audio_chunksize = afe_handle->get_feed_chunksize(afe_data); // 获取帧长度
    int total_channel_num = afe_handle->get_total_channel_num(afe_data); //获取总共通道数，麦克风通道+参考通道
    int nch = afe_handle->get_channel_num(afe_data); // 获取声道数
    int feed_channel = get_feed_channel(); // 获取ADC输入通道数,对应麦克风有几路输入
    ESP_LOGI(TAG, "total_channel_num=%d nch=%d feed_channel=%d", total_channel_num, nch, feed_channel);
    assert(nch <= feed_channel);
    //分配feed通道内存，通道数是total_channel_num
    int16_t *i2s_buff = (int16_t*)heap_caps_malloc(audio_chunksize * sizeof(int16_t) * total_channel_num, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM); // 分配获取I2S数据的缓存大小
    assert(i2s_buff);

    // IPAddress remoteIP(192, 168, 2, 153);  // 替换为实际的目标IP地址
    // WiFiClient client;
    // if (!client.connect(remoteIP, 8888)) {
    //     vTaskDelete(NULL);
    //     ESP_LOGW(TAG, "connect webserver failure");
    //     return;
    // }
    while (task_flag) {
        // get_feed_data(false, i2s_buff, bufferLen);  // 获取I2S数据
        size_t len = mic.read(reinterpret_cast<char*>(i2s_buff), audio_chunksize * sizeof(int16_t));
        // i2s_channel_read(rx_handle, i2s_buff, buffer_len, &bytes_read, 100);
        // esp_err_t result = i2s_read(I2S_NUM_0, i2s_buff, audio_chunksize * sizeof(int16_t), &bytes_read, portMAX_DELAY);
        // client.write((uint8_t*)i2s_buff, bytes_read);
        //调整feed通道
        for (int  i = audio_chunksize - 1; i >= 0; i--) {
            i2s_buff[i * 2 + 1] = 0;
            i2s_buff[i * 2 + 0] = i2s_buff[i];
        }

        afe_handle->feed(afe_data, i2s_buff); // 把获取到的I2S数据输入给afe_data
    }
    afe_handle->destroy(afe_data);
    if (i2s_buff) {
        free(i2s_buff);
        i2s_buff = NULL;
    }
    vTaskDelete(NULL);
}

void Sr::detect_Task(void *arg)
{
    esp_afe_sr_data_t *afe_data = (esp_afe_sr_data_t*)arg;  // 接收参数
    int afe_chunksize = afe_handle->get_fetch_chunksize(afe_data);  // 获取fetch帧长度
    char *mn_name = esp_srmodel_filter(models, ESP_MN_PREFIX, ESP_MN_CHINESE); // 初始化命令词模型
    printf("multinet:%s\n", mn_name); // 打印命令词模型名称
    esp_mn_iface_t *multinet = esp_mn_handle_from_name(mn_name);
    model_iface_data_t *model_data = multinet->create(mn_name, 6000);  // 设置唤醒后等待事件 6000代表6000毫秒
    esp_mn_commands_clear(); // 清除当前的命令词列表
    esp_mn_commands_add(1, "bo fang yin yue"); // 播放音乐
    esp_mn_commands_add(2, "zan ting"); // 暂停
    esp_mn_commands_add(3, "ji xu"); // 继续
    esp_mn_commands_add(4, "shang yi shou"); // 上一首
    esp_mn_commands_add(5, "xia yi shou"); // 下一首
    esp_mn_commands_add(6, "sheng yin da yi dian"); // 声音大一点
    esp_mn_commands_add(7, "sheng yin xiao yi dian"); // 声音小一点
    esp_mn_commands_update(); // 更新命令词
    int mu_chunksize = multinet->get_samp_chunksize(model_data);  // 获取samp帧长度
    assert(mu_chunksize == afe_chunksize);

    // 打印所有的命令
    multinet->print_active_speech_commands(model_data);
    printf("------------detect start------------\n");

    while (task_flag) {
        afe_fetch_result_t* res = afe_handle->fetch(afe_data); // 获取模型输出结果
        // ESP_LOGW(TAG, "ret_value=%d", res->ret_value);
        if (!res || res->ret_value == ESP_FAIL) {
            printf("fetch error!\n");
            break;
        }
        if (res->wakeup_state == WAKENET_DETECTED) {
            printf("WAKEWORD DETECTED\n");
	        multinet->clean(model_data);  // clean all status of multinet
        } else if (res->wakeup_state == WAKENET_CHANNEL_VERIFIED) {  // 检测到唤醒词
            // play_voice = -1;
            afe_handle->disable_wakenet(afe_data);  // 关闭唤醒词识别
            detect_flag = 1; // 标记已检测到唤醒词
            // ai_gui_in(); // AI人出现
            printf("AFE_FETCH_CHANNEL_VERIFIED, channel index: %d\n", res->trigger_channel_id);
        }

        if (detect_flag == 1) {
            esp_mn_state_t mn_state = multinet->detect(model_data, res->data); // 检测命令词

            if (mn_state == ESP_MN_STATE_DETECTING) {
                continue;
            }

            if (mn_state == ESP_MN_STATE_DETECTED) { // 已检测到命令词
                esp_mn_results_t *mn_result = multinet->get_results(model_data); // 获取检测词结果
                for (int i = 0; i < mn_result->num; i++) { // 打印获取到的命令词
                    printf("TOP %d, command_id: %d, phrase_id: %d, string:%s prob: %f\n", 
                    i+1, mn_result->command_id[i], mn_result->phrase_id[i], mn_result->string, mn_result->prob[i]);
                }
                // 根据命令词 执行相应动作
                switch (mn_result->command_id[0])
                {
                    // case 1: // bo fang yin yue 播放音乐
                    //     ai_play();
                    //     break;
                    // case 2: // zan ting 暂停
                    //     ai_pause();
                    //     break;
                    // case 3: // ji xu 继续
                    //     ai_resume();
                    //     break;
                    // case 4: // shang yi shou 上一首
                    //     ai_prev_music();
                    //     break;
                    // case 5: // xia yi shou 下一首
                    //     ai_next_music();
                    //     break;
                    // case 6: // sheng yin da yi dian 声音大一点
                    //     ai_volume_up();
                    //     break;
                    // case 7: // sheng yin xiao yi dian 声音小一点
                    //     ai_volume_down();
                        break;

                    default:
                        break;
                }
                printf("\n-----------listening-----------\n");
            }

            if (mn_state == ESP_MN_STATE_TIMEOUT) { // 达到最大检测命令词时间
                esp_mn_results_t *mn_result = multinet->get_results(model_data);
                printf("timeout, string:%s\n", mn_result->string);
                afe_handle->enable_wakenet(afe_data);  // 重新打开唤醒词识别
                detect_flag = 0; // 清除标记
                printf("\n-----------awaits to be waken up-----------\n");
                // ai_gui_out(); // AI人退出
                continue;
            }
        }
    }
    if (model_data) {
        multinet->destroy(model_data);
        model_data = NULL;
    }
    printf("detect exit\n");
    vTaskDelete(NULL);
}


//事件处理函数
void Sr::init_i2s(){

    // i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    // /* Allocate a new TX channel and get the handle of this channel */
    // i2s_new_channel(&chan_cfg, NULL, &rx_handle);

    // i2s_std_config_t std_cfg = {
    //     .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(16),
    //     .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
    //     .gpio_cfg = {
    //         .mclk = I2S_GPIO_UNUSED,
    //         .bclk = GPIO_NUM_4,
    //         .ws = GPIO_NUM_5,
    //         .dout = GPIO_NUM_6,
    //         .din = I2S_GPIO_UNUSED,
    //         .invert_flags = {
    //             .mclk_inv = false,
    //             .bclk_inv = false,
    //             .ws_inv = false,
    //         },
    //     },
    // };
    // i2s_channel_init_std_mode(rx_handle, &std_cfg);
    // i2s_channel_enable(rx_handle);
    
    // i2s_config_t i2s_config = {
    //     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    //     .sample_rate = 16000,
    //     .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    //     .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    //     .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
    //     .intr_alloc_flags = 0,
    //     .dma_buf_count = 8,
    //     .dma_buf_len = bufferLen,
    //     .use_apll = false          // 分配中断标志
    // };
    // i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    // i2s_pin_config_t pin_config = {
    //     .bck_io_num = MAX98375_BCLK_IO1,            // BCLK引脚号
    //     .ws_io_num = MAX98375_LRCLK_IO1,             // LRCK引脚号
    //     .data_out_num = -1, // DATA引脚号
    //     .data_in_num = MAX98375_DOUT_IO1,           // DATA_IN引脚号
    // };
    // i2s_set_pin(I2S_NUM_0, &pin_config);
    // i2s_start(I2S_NUM_0);
    
}



void Sr::setup(void)
{
    mic.setup(16000, I2S_DATA_BIT_WIDTH_16BIT);
    models = esp_srmodel_init("model"); // 获取模型 名称“model”和分区表中装载模型的名称一致

    afe_handle = (esp_afe_sr_iface_t *)&ESP_AFE_SR_HANDLE;  // 先配置afe句柄 随后才可以调用afe接口
    afe_config_t afe_config =  { 
        .aec_init = false, 
        .se_init = true, 
        .vad_init = true, 
        .wakenet_init = true, 
        .voice_communication_init = false, 
        .voice_communication_agc_init = false, 
        .voice_communication_agc_gain = 15, 
        .vad_mode = VAD_MODE_3, 
        .wakenet_model_name = NULL, 
        .wakenet_model_name_2 = NULL, 
        .wakenet_mode = DET_MODE_2CH_90, 
        .afe_mode = SR_MODE_LOW_COST, 
        .afe_perferred_core = 0, 
        .afe_perferred_priority = 5, 
        .afe_ringbuf_size = 50, 
        .memory_alloc_mode = AFE_MEMORY_ALLOC_MORE_PSRAM, 
        .afe_linear_gain = 1.0, 
        .agc_mode = AFE_MN_PEAK_AGC_MODE_2, 
        .pcm_config = {
            .total_ch_num = 2, 
            .mic_num = 1, 
            .ref_num = 1, 
            .sample_rate = 16000, 
        },
        .debug_init = false, 
        .debug_hook = {{AFE_DEBUG_HOOK_MASE_TASK_IN, NULL}, {AFE_DEBUG_HOOK_FETCH_TASK_IN, NULL}}, 
        .afe_ns_mode = NS_MODE_SSP, 
        .afe_ns_model_name = NULL, 
    }; // 配置afe

    afe_config.wakenet_model_name = esp_srmodel_filter(models, ESP_WN_PREFIX, NULL); // 配置唤醒模型 必须在create_from_config之前配置
    afe_data = afe_handle->create_from_config(&afe_config); // 创建afe_data
    ESP_LOGI(TAG, "wakenet:%s", afe_config.wakenet_model_name); // 打印唤醒名称

    task_flag = 1;
    xTaskCreatePinnedToCore(&Sr::detectDelegate, "detect", 8 * 1024, (void*)this, 5, NULL, 1); 
    xTaskCreatePinnedToCore(&Sr::feedDelegate, "feed", 8 * 1024, (void*)this, 5, NULL, 0);
}
