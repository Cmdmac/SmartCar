#include "Arduino.h"
#include "Command.h"
// #include "Led.h"
// #include "Servo2.h"
#include "DataChannel.h"
#include "Net.h"
// #include "Ir.h"
#include "QMI8658.h"
#include "PCA9557.h"
#include "Camera.h"
#include "TFT_SPI.h"
#include "logo_en_240x240_lcd.h"
#include "demos/lv_demos.h"
#include "Sr.h"
// Led led;

#include "driver/i2s.h"
#define MAX98375_BCLK_IO1 5
#define MAX98375_LRCLK_IO1 4
#define MAX98375_DOUT_IO1 6
// const char* host = "192.168.1.4"; // 电脑的IP地址
// const int port = 8888; // 监听的端口
#define bufferLen 512

extern Net net;

// Ir ir;
QMI8658 qmi8658;
PCA9557 io(0x19, &Wire); // 0x19 for iFarm4G board

Camera camera;

TFT_SPI tft;
Sr sr;

static void Init_i2s(){
  i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
      .sample_rate = 16000,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
      .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_STAND_I2S),
      .intr_alloc_flags = 0,
      .dma_buf_count = 8,
      .dma_buf_len = bufferLen,
      .use_apll = false          // 分配中断标志
  };
  ESP_ERROR_CHECK(i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL));
  i2s_pin_config_t pin_config = {
      .bck_io_num = MAX98375_BCLK_IO1,            // BCLK引脚号
      .ws_io_num = MAX98375_LRCLK_IO1,             // LRCK引脚号
      .data_out_num = -1, // DATA引脚号
      .data_in_num = MAX98375_DOUT_IO1,           // DATA_IN引脚号
  };
  ESP_ERROR_CHECK(i2s_set_pin(I2S_NUM_0, &pin_config));
//   ESP_ERROR_CHECK(i2s_start(I2S_NUM_0));
  
}

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_mac.h"
// #include "nvs_flash.h"
#include "sys/socket.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "netdb.h"
#include "driver/i2s.h"
#include "arpa/inet.h"

#include <WiFiUdp.h>


#define TAG "app"
#define HOST_IP_ADDR "192.168.2.153"
#define HOST_PORT 8888
#define bufferLen 512

int16_t sBuffer[512];
 
static const char *payload = "Hello from ESP32\r\n";
static TaskHandle_t client_task_handle;
 
WiFiUDP udp;
IPAddress remoteIP(192, 168, 2, 153);  // 替换为实际的目标IP地址
WiFiClient client;

static void udp_client_task(void *args)
{
  ESP_LOGW("main", "connect %d", client.connect("192.168.2.153", 8888));
    ESP_LOGI(TAG, "Socket created, sending to %s:%d", HOST_IP_ADDR, HOST_PORT);
 
    while (client.connected()) {
        // 开始发送数据包
        size_t bytesIn = 0;
        esp_err_t result = i2s_read(I2S_NUM_0, &sBuffer, bufferLen * sizeof(int16_t), &bytesIn, portMAX_DELAY);
        
        size_t r = client.write((uint8_t*)sBuffer, bytesIn);
        ESP_LOGW("main", "write r=%d", r);
        delay(10);
 
        // c
    }
 ESP_LOGW("main", "exit");
    vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);
  Serial.println("setup");
  // Init_i2s();

  net.setUpWifi();
  // 启动UDP
  // xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, &client_task_handle);
  // ir.setup();
  // ir.initIrDatas();
  // ir.startLearn();
  // camera.setUp();
  // camera.startStreamServer(); 
//   Serial.println(Wire.begin(14, 13));

  // tft.setup();
  sr.setup();

  // tft.drawPicture(0, 0, 240, 240, (const unsigned char *) logo_en_240x240_lcd);

  // lv_demo_benchmark(); 
  // qmi8658.setUp();
  // scanI2CDevices();

  // io.pinMode(0, OUTPUT);
  // io.pinMode(1, OUTPUT);
  // io.pinMode(6, INPUT);

  // io.pinMode(7, OUTPUT);

  // io.digitalWrite(0, HIGH);
  // io.digitalWrite(1, HIGH);
  // io.digitalWrite(7, HIGH);
}

void loop() {
  // Serial.println("Hello world!");
  delay(1);
  // // qmi8658.loop();
  // QMI8658Data data;
  // io.digitalWrite(7, LOW);
  // vTaskDelay(1000 / portTICK_PERIOD_MS);  // 延时1000ms
  // io.digitalWrite(7, HIGH);
  // qmi8658.getAngleFromAcc(&data);   // 获取XYZ轴的倾角
  // // 输出XYZ轴的倾角
  // ESP_LOGI("Main", "angle_x = %.1f  angle_y = %.1f angle_z = %.1f", data.AngleX, data.AngleY, data.AngleZ);
  // Serial.println(io.digitalRead(6));
}

