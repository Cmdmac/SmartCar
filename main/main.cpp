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

// Led led;
extern Net net;

// Ir ir;
QMI8658 qmi8658;
PCA9557 io(0x19, &Wire); // 0x19 for iFarm4G board

Camera camera;

void setup() {
  Serial.begin(115200);
  Serial.println("setup");
  net.setUpWifi();
  // ir.setup();
  // ir.initIrDatas();
  // ir.startLearn();
  // Serial.println(Wire.begin(4, 5));
  camera.setUp();
  camera.startStreamServer();

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
