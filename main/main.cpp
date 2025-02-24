#include "Arduino.h"
#include "Led.h"
#include "Servo2.h"
#include "DataChannel.h"
#include "net.h"

Led led;
extern Net net;

void setup() {
  Serial.begin(115200);
  Serial.println("setup");
  net.setUpWifi();
}

void loop() {
  Serial.println("Hello world!");
  delay(1000);
}
