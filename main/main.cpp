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

// #include "driver/i2s.h"
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

