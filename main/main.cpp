#include "Arduino.h"
#include "Led.h"
#include "Servo2.h"
#include "DataChannel.h"
#include "Net.h"
#include "Ir.h"
#include "QMI8658.h"
Led led;
extern Net net;

Ir ir;
QMI8658 qmi8658;

void setup() {
  Serial.begin(115200);
  Serial.println("setup");
  net.setUpWifi();
  // ir.setup();
  // ir.initIrDatas();
  // ir.startLearn();
  qmi8658.setUp();
}

void loop() {
  // Serial.println("Hello world!");
  // delay(1000);
  // qmi8658.loop();
    QMI8658Data data;
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // 延时1000ms
    qmi8658.readAccAndGry(&data);   // 获取XYZ轴的倾角
    // 输出XYZ轴的倾角
    ESP_LOGI("Main", "angle_x = %.1f  angle_y = %.1f angle_z = %.1f", data.AngleX, data.AngleY, data.AngleZ);

}
