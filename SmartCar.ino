#include "Motor.h"



// 定义电机引脚
const int motor1Pin1 = 2;
const int motor1Pin2 = 3;
const int motor2Pin1 = 4;
const int motor2Pin2 = 5;

// 定义电机速度控制引脚（如果使用 PWM 调速）
const int motor1SpeedPin = 9;
const int motor2SpeedPin = 10;

void setup() {
  // 设置电机控制引脚为输出模式
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // 如果使用 PWM 调速，设置速度控制引脚为输出模式
  pinMode(motor1SpeedPin, OUTPUT);
  pinMode(motor2SpeedPin, OUTPUT);
}

void forward() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);

  // 如果使用 PWM 调速，设置电机速度
  analogWrite(motor1SpeedPin, 200);  // 速度值可根据需要调整
  analogWrite(motor2SpeedPin, 200);
}

void backward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);

  // 如果使用 PWM 调速，设置电机速度
  analogWrite(motor1SpeedPin, 200);  // 速度值可根据需要调整
  analogWrite(motor2SpeedPin, 200);
}

void left() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);

  // 如果使用 PWM 调速，设置电机速度
  analogWrite(motor1SpeedPin, 100);  // 速度值可根据需要调整
  analogWrite(motor2SpeedPin, 200);
}

void right() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);

  // 如果使用 PWM 调速，设置电机速度
  analogWrite(motor1SpeedPin, 200);  // 速度值可根据需要调整
  analogWrite(motor2SpeedPin, 100);
}

void stop() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}

void loop() {
  // 这里可以根据您的需求调用不同的函数来控制小车动作
  forward();  // 前进
  // delay(1000);
  // stop();  // 停止
  delay(1000);
  backward();  // 后退
  // delay(1000);
  // stop();  // 停止
  delay(1000);
  left();  // 左转
  // delay(1000);
  // stop();  // 停止
  delay(1000);
  right();  // 右转
  // delay(1000);
  // stop();  // 停止
  delay(1000);
}