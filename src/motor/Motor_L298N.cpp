#include "Motor_L298N.h"
#include <Arduino.h>

Motor_L298N::Motor_L298N(int pinPos, int pinNeg, int pinPwm, int pinStby): Motor(pinPos, pinNeg, pinPwm), pinStby(pinStby) {

}

void Motor_L298N::forward() {
  // Serial.println("forward");
  // Serial.println(this->speed);
  digitalWrite(this->pinPos, HIGH);
  digitalWrite(this->pinNeg, LOW);
  analogWrite(this->pinPwm, this->speed  * 255);  

}

void Motor_L298N::backward() {
  digitalWrite(this->pinPos, LOW);
  digitalWrite(this->pinNeg, HIGH);
}

void Motor_L298N::stop() {
    // Serial.println("stop");
  digitalWrite(this->pinPos, LOW);
  digitalWrite(this->pinNeg, LOW);
  analogWrite(this->pinPwm, 0);  
}