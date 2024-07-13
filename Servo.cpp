#include "Servo.h"
#include <Arduino.h>

Servo::Servo(int pin) : pinPwm(pin) {

}

void Servo::to(int angle) {
  //发送50个脉冲
  for (int i = 0; i < 50; i++) {
    int pulsewidth = (angle * 11) + 500; //将角度转化为500-2480的脉宽值
    digitalWrite(this->pinPwm, HIGH);   //将舵机接口电平至高
    delayMicroseconds(pulsewidth);  //延时脉宽值的微秒数
    digitalWrite(this->pinPwm, LOW);    //将舵机接口电平至低
    delayMicroseconds(20000 - pulsewidth);
  }
}