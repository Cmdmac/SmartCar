#include "Motor.h"



// 定义电机引脚
const int motor1Pin1 = 2;
const int motor1Pin2 = 3;
const int motor2Pin1 = 4;
const int motor2Pin2 = 5;

// 定义电机速度控制引脚（如果使用 PWM 调速）
const int motor1SpeedPin = 9;
const int motor2SpeedPin = 10;

Motor motor(2, 3, 9);

void setup() {
  // // 设置电机控制引脚为输出模式
  // pinMode(motor1Pin1, OUTPUT);
  // pinMode(motor1Pin2, OUTPUT);
  // pinMode(motor2Pin1, OUTPUT);
  // pinMode(motor2Pin2, OUTPUT);

  // // 如果使用 PWM 调速，设置速度控制引脚为输出模式
  // pinMode(motor1SpeedPin, OUTPUT);
  // pinMode(motor2SpeedPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  
  motor.forward();
  delay(5000);
  // motor.speedUp();
  // delay(5000);
  // motor.speedUp();
  // delay(5000);
  // motor.speedUp();
  // delay(5000);
  // motor.speedUp();
  // delay(5000);
  // motor.speedDown();
  // delay(5000);
  // motor.speedDown();
  // delay(5000);
  // motor.speedDown();
  // delay(5000);
  // motor.speedDown();
  // delay(5000);
  // motor.speedDown();
}