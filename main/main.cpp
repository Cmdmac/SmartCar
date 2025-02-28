#include "Arduino.h"
// #include "Led.h"
// #include "Servo2.h"
// #include "DataChannel.h"
#include "Net.h"
// #include "Ir.h"
#include "QMI8658.h"
#include "PCA9557.h"

// Led led;
extern Net net;

// Ir ir;
QMI8658 qmi8658;
PCA9557 io(0x19, &Wire); // 0x19 for iFarm4G board

void setup() {
  Serial.begin(115200);
  Serial.println("setup");
  // net.setUpWifi();
  // ir.setup();
  // ir.initIrDatas();
  // ir.startLearn();
  // qmi8658.setUp();
  Serial.println(Wire.begin(4, 5));

  Serial.println("\nI2C Scanner");
  Serial.println("Scanning...");

  uint8_t error, address;
  int nDevices;

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    // 尝试连接到当前地址
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }

  io.pinMode(0, OUTPUT);
  io.digitalWrite(0, HIGH);
}

void loop() {
  Serial.println("Hello world!");
  // delay(1000);
  // qmi8658.loop();
    // QMI8658Data data;
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // 延时1000ms
    // qmi8658.getAngleFromAcc(&data);   // 获取XYZ轴的倾角
    // // 输出XYZ轴的倾角
    // ESP_LOGI("Main", "angle_x = %.1f  angle_y = %.1f angle_z = %.1f", data.AngleX, data.AngleY, data.AngleZ);

}
