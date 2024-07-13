#include "Motor_TB6612FNG.h"
#include "Car.h"

Motor_TB6612FNG l = Motor_TB6612FNG(2, 3, 9, 7);
Motor_TB6612FNG r = Motor_TB6612FNG(4, 5, 10, 7);

Car car = Car(&l, &r);

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
  
  car.forward();
  
  delay(5000);

  car.backward();
  //  car.speedUp();
   delay(5000);
   car.speedDown();
   delay(5000);
  //  car.speedDown();
  //  delay(5000);
  //  car.speedDown();
  //  delay(5000);
   car.stop();
   delay(5000);
  // car.faceTo(90);
  // for (int i = 0; i <= 180; i+= 1) {
  //   car.faceTo(i);
  //   // delay(5);
  // }
  //  for (int i = 180; i > 0; i-= 1) {
  //   car.faceTo(i);
  //   // delay(5);
  // }
// 
}
