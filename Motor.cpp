#include<Arduino.h>
#include"Motor.h"


Motor::Motor(int pinPos, int pinNeg, int pinPwm) {
  this->pinPos = pinPos;
  this->pinNeg = pinNeg;
  this->pinPwm = pinPwm;
  this->speed = 0.1;
  pinMode(this->pinPos, OUTPUT);
  pinMode(this->pinNeg, OUTPUT);
  pinMode(this->pinPwm, OUTPUT);
}

void Motor::forward() {
  digitalWrite(this->pinPos, HIGH);
  digitalWrite(this->pinNeg, LOW);
}

void Motor::backward() {
  digitalWrite(this->pinPos, LOW);
  digitalWrite(this->pinNeg, HIGH);
}

void Motor::stop() {
  digitalWrite(this->pinPos, LOW);
  digitalWrite(this->pinNeg, LOW);
}

// public void Motor::left();
// public void Motor::right();
void Motor::speedUp() {
  this->speed = this->speed + 0.1;
  if (this->speed > 1) {
    this->speed = 1;
  }
  Serial.println(this->speed);
  analogWrite(this->pinPwm, this->speed);  
}

void Motor::speedDown() {
  this->speed = this->speed - 0.1;
  if (this->speed <= 0) {
    this->speed = 0.1;
  }
  analogWrite(this->pinPwm, this->speed);  
}

void Motor::setSpeed(float speed) {
  analogWrite(this->pinPwm, this->speed);  
}