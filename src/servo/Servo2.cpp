#include "Servo2.h"
#include <Arduino.h>

// #define SERVO_PIN   13
#define MAX_WIDTH   2500
#define MIN_WIDTH   500
Servo2::Servo2(int pin) : pinPwm(pin) {
//  // 分配硬件定时器
//   ESP32PWM::allocateTimer(0);
//   // 设置频率
//   my_servo.setPeriodHertz(50);
//   // 关联 servo 对象与 GPIO 引脚，设置脉宽范围
//   my_servo.attach(pin, MIN_WIDTH, MAX_WIDTH);

}

void Servo2::to(int angle) {
  //发送50个脉冲
  // for (int i = 0; i < 50; i++) {
  //   int pulsewidth = (angle * 11) + 500; //将角度转化为500-2480的脉宽值
  //   digitalWrite(this->pinPwm, HIGH);   //将舵机接口电平至高
  //   delayMicroseconds(pulsewidth);  //延时脉宽值的微秒数
  //   digitalWrite(this->pinPwm, LOW);    //将舵机接口电平至低
  //   delayMicroseconds(20000 - pulsewidth);
  // }
  // this->my_servo.write(angle);
  
}