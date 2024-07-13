#include "Motor.h"
#include "Car.h"


// 定义电机引脚
const int motor1Pin1 = 2;
const int motor1Pin2 = 3;
const int motor2Pin1 = 4;
const int motor2Pin2 = 5;

// 定义电机速度控制引脚（如果使用 PWM 调速）
const int motor1SpeedPin = 9;
const int motor2SpeedPin = 10;

Car car = Car(2, 3, 8, 4, 5, 9, 6);

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
  
  // car.forward();
  // delay(5000);
  //  car.speedUp();
  //  delay(5000);
  //  car.speedDown();
  //  delay(5000);
  //  car.speedDown();
  //  delay(5000);
  //  car.speedDown();
  //  delay(5000);
  //  car.stop();
  //  delay(5000);
  // car.faceTo(90);
  for (int i = 0; i <= 180; i+= 1) {
    car.faceTo(i);
    // delay(5);
  }
   for (int i = 180; i > 0; i-= 1) {
    car.faceTo(i);
    // delay(5);
  }
// 
}
