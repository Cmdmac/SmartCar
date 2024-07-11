#include"Motor.h"
public Motor::Motor(int pinPos, int pinNeg, int pinPwm) {
  this.pinPos = pinPos;
  this.pinNeg = pinNeg;
  this.pinPwm = pinPwm;
  pinMode(this.pinPos, OUTPUT);
  pinMode(this.pinNeg, OUTPUT);
  pinMode(this.pinPwm, OUTPUT);
}
public void Motor::forward() {
  digitalWrite(this.pinPos, HIGH);
  digitalWrite(this.pinNeg, LOW);
}
public void Motor::backward() {
  digitalWrite(this.pinPos, LOW);
  digitalWrite(this.pinNeg, HIGH);
}
public void Motor::stop() {
  digitalWrite(this.pinPos, LOW);
  digitalWrite(this.pinNeg, LOW);
}

// public void Motor::left();
// public void Motor::right();
public void Motor::speedUp() {
  this.speed = this.speed + 100;
  if (this.speed > 1000) {
    this.speed = 1000;
  }
  analogWrite(this.pinPwm, this.speed);  // 速度值可根据需要调整
}

public void Motor::speedDown() {
  this.speed = this.speed - 100;
  if (this.speed <= 0) {
    this.speed = 100;
  }
  analogWrite(this.pinPwm, this.speed);  // 速度值可根据需要调整
}