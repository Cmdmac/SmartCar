#include "Motor_TB6612FNG.h"
#include <Arduino.h>

Motor_TB6612FNG::Motor_TB6612FNG(int pinPos, int pinNeg, int pinPwm, int pinStby): Motor(pinPos, pinNeg, pinPwm), pinStby(pinStby) {
  pinMode(this->pinPos, OUTPUT);
  pinMode(this->pinNeg, OUTPUT);
  pinMode(this->pinPwm, OUTPUT);
  pinMode(this->pinStby, OUTPUT);
  digitalWrite(this->pinStby, HIGH);
}

void Motor_TB6612FNG::forward() {
  
  // Serial.println("forward");
  // Serial.println(this->speed);
  // Serial.println(this->speed);
  digitalWrite(this->pinStby, HIGH);

  digitalWrite(this->pinPos, HIGH);
  digitalWrite(this->pinNeg, LOW);
  // analogWrite(this->pinPwm, this->speed  * 255);  

}

void Motor_TB6612FNG::backward() {
  // Serial.println("backward");
  // Serial.println(this->speed);
  digitalWrite(this->pinPos, LOW);
  digitalWrite(this->pinNeg, HIGH);
  // analogWrite(this->pinPwm, this->speed  * 255);  
}

void Motor_TB6612FNG::stop() {
    // Serial.println("stop");
  digitalWrite(this->pinPos, LOW);
  digitalWrite(this->pinNeg, LOW);
  analogWrite(this->pinPwm, 0);  
}