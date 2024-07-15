#include "Motor_TB6612FNG.h"
#include "Car.h"
#include "UltraSound.h"

Motor_TB6612FNG l = Motor_TB6612FNG(2, 3, 9, 7);
Motor_TB6612FNG r = Motor_TB6612FNG(4, 5, 11, 7);
Servo s = Servo(8);
UltraSound us = UltraSound(12, 13);
Car car = Car(&l, &r, &s, &us);


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

  // car.backward();
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
  // for (int i = 0; i <= 180; i+= 1) {
  //   car.faceTo(i);
  //   // delay(5);
  // }
  //  for (int i = 180; i > 0; i-= 1) {
  //   car.faceTo(i);
  //   // delay(5);
  // }

  //  digitalWrite(trigPin, LOW);  // 先将触发引脚拉低 2μs 以上
  // delayMicroseconds(2);

  // digitalWrite(trigPin, HIGH);  // 给触发引脚至少 10μs 的高电平脉冲
  // delayMicroseconds(10);
  // digitalWrite(trigPin, LOW);

  // long duration = pulseIn(echoPin, HIGH);  // 测量回声引脚高电平持续的时间

  // // 根据声速计算距离（声速 340m/s，往返距离）
  // long distance = duration * 0.034 / 2;

  // if (duration == 0) {
  //   Serial.println("Out of range");
  // } else {
  //   Serial.print("Distance: ");
  //   Serial.print(distance);
  //   Serial.println(" cm");
  // }

  car.getDistance();
  delay(100);  // 稍作延迟

// 
}
