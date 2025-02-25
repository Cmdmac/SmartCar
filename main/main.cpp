#include "Arduino.h"
#include "Led.h"
#include "Servo2.h"
#include "DataChannel.h"
#include "Net.h"
#include "Ir.h"
Led led;
extern Net net;

Ir ir;
void setup() {
  Serial.begin(115200);
  Serial.println("setup");
  net.setUpWifi();
  ir.setup();
  ir.startLearn();
}

void loop() {
  Serial.println("Hello world!");
  delay(1000);
}
