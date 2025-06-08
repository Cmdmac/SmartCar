// #include "Arduino.h"
#include "Command.h"
#include "freertos/FreeRTOS.h"  // 包含 FreeRTOS 基础头文件
#include "freertos/task.h"      // 包含任务相关头文件
// #include "Wire.h"
// #include "Led.h"
// #include "Servo2.h"
// #include "DataChannel.h"
// #include "Net.h"
// #include "Ir.h"
// #include "QMI8658.h"
// #include "PCA9557.h"
// #include "Camera.h"
// #include "TFT_SPI.h"
#include "logo_en_240x240_lcd.h"
// #include "demos/lv_demos.h"
// #include "Sr.h"
// #include <TCA6408.h>
// #include "power_management.h"
// #include "Battery.h"
// #include "MAX1704X.h"
// Led led;
#include "Config.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "es8311_audio_codec.h"


// #include "driver/i2s.h"
#define MAX98375_BCLK_IO1 5
#define MAX98375_LRCLK_IO1 4
#define MAX98375_DOUT_IO1 6
// const char* host = "192.168.1.4"; // 电脑的IP地址
// const int port = 8888; // 监听的端口
#define bufferLen 512

// extern Net net;

// Ir ir;
// QMI8658 qmi8658;
// PCA9557 io(0x19, &Wire); // 0x19 for iFarm4G board

// Camera camera;

// TFT_SPI tft(BSP_LCD_SPI_MOSI, BSP_LCD_SPI_CLK, BSP_LCD_SPI_CS, BSP_LCD_DC, BSP_LCD_RST, BSP_LCD_BACKLIGHT);
// Sr sr;

// MAX1704X max1704x(1.0);
// Battery battery(GPIO_NUM_2);

// const TCA6408::DeviceAddress DEVICE_ADDRESS = TCA6408::DEVICE_ADDRESS_0;
// uint8_t RESET_PIN = 0;
// TCA6408 tca6408;

i2c_master_bus_handle_t i2c_bus_;



void setup() {
  // Serial.begin(115200);
  // Serial.println("setup");
  // Init_i2s();

    // net.setUpWifi();
  // 启动UDP
  // xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, &client_task_handle);
  // ir.setup();
  // ir.initIrDatas();
  // ir.startLearn();
  // camera.setUp();
      // camera.startStreamServer(); 
      
  // ESP_LOGI("Main", "wire begin %d", Wire.begin(12, 13));


  // max1704x.begin(&Wire, 0x36); // 0x36 for iFarm4G board
  // max1704x.print();

  // battery.setup();


  // tca6408.setup(Wire, DEVICE_ADDRESS);
  // tca6408.setResetPin(RESET_PIN);

  // Serial.print("p0: ");
  // Serial.println(tca6408.digitalRead(TCA6408::P0));
  // tca6408.pinMode(TCA6408::P0, OUTPUT);

  // tca6408.digitalWrite(TCA6408::P0, 1);
  // Serial.print("P0: ");
  // Serial.println(tca6408.digitalRead(TCA6408::P0));

  // Serial.print("p7: ");
  // Serial.println(tca6408.digitalRead(TCA6408::P7));
  // tca6408.pinMode(TCA6408::P7, OUTPUT);

  // tca6408.digitalWrite(TCA6408::P7, 1);
  // Serial.print("P7: ");
  // Serial.println(tca6408.digitalRead(TCA6408::P7));

  // tca6408.printPinStates();

  // tft.setup();
  // tft.setBrightness(100);

  // battery_voltage_monitor_start();
  // tft.fillScreen(0xff0);
  // sr.setup();

  int x = (BSP_LCD_H_RES - 240.0) / 2;
  int y = (BSP_LCD_V_RES - 240.0) / 2;
  // Serial.println(x);
  // Serial.println(y);
  // tft.drawPicture(x,  y, x + 240, y + 240, (const unsigned char *) logo_en_240x240_lcd);
    
  // tft.fillScreen(0xf00);

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
  // scanI2CDevices();
  i2c_master_bus_config_t i2c_bus_cfg = {
      .i2c_port = (i2c_port_t)1,
      .sda_io_num = GPIO_NUM_12,
      .scl_io_num = GPIO_NUM_13,
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .glitch_ignore_cnt = 7,
      .intr_priority = 0,
      .trans_queue_depth = 0,
      .flags = {
          .enable_internal_pullup = 1,
      },
  };
  ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_cfg, &i2c_bus_));

  Es8311AudioCodec es8311(
    i2c_bus_, // I2C master handle
    I2C_NUM_0, // I2C port
    16000, // Input sample rate
    16000, // Output sample rate
    GPIO_NUM_14, // MCLK
    GPIO_NUM_21, // BCLK
    GPIO_NUM_47, // WS
    GPIO_NUM_45, // DOUT
    GPIO_NUM_48, // DIN
    GPIO_NUM_NC, // PA pin (not used)
    0x7E // ES8311 I2C address
);
  // es8311.CreateDuplexChannels(
  //   GPIO_NUM_14, // MCLK
  //   GPIO_NUM_21, // BCLK
  //   GPIO_NUM_47, // WS
  //   GPIO_NUM_45, // DOUT
  //   GPIO_NUM_48, // DIN
  //   GPIO_NUM_NC, // PA pin
  //   0x7E      // ES8311 I2C address
  // );
  // es8311.EnableInput(true);
  // es8311.EnableOutput(true);
}

void loop() {
  // Serial.println("Hello world!");
    // Serial.println(battery.detect());
//  ESP_LOGI("Main", "loop");
    // delay(1000);
  // // qmi8658.loop();
  // QMI8658Data data;
  // io.digitalWrite(7, LOW);
  // vTaskDelay(1000 / portTICK_PERIOD_MS);  // 延时1000ms
  // io.digitalWrite(7, HIGH);
  // qmi8658.getAngleFromAcc(&data);   // 获取XYZ轴的倾角
  // // 输出XYZ轴的倾角
  // ESP_LOGI("Main", "angle_x = %.1f  angle_y = %.1f angle_z = %.1f", data.AngleX, data.AngleY, data.AngleZ);
  // Serial.println(io.digitalRead(6));
  vTaskDelay(pdMS_TO_TICKS(1));  // 至少释放1ms CPU时间
}

