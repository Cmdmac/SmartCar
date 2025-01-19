#include "UltraSound.h"
#include <Arduino.h>

UltraSound::UltraSound(int pinTri, int pinEco) : pinTri(pinTri), pinEco(pinEco) {
  pinMode(this->pinTri, OUTPUT); // 将触发引脚设置为输出模式
  pinMode(this->pinEco, INPUT);; // 将回声引脚设置为输入模式
}

int UltraSound::getDistance() {
  digitalWrite(this->pinTri, LOW);  // 先将触发引脚拉低 2μs 以上
  delayMicroseconds(2);

  digitalWrite(this->pinTri, HIGH);  // 给触发引脚至少 10μs 的高电平脉冲
  delayMicroseconds(10);
  digitalWrite(this->pinTri, LOW);

  long duration = pulseIn(this->pinEco, HIGH);  // 测量回声引脚高电平持续的时间

  // 根据声速计算距离（声速 340m/s，往返距离）
  long distance = duration * 0.034 / 2;

  if (duration == 0) {
    Serial.println("Out of range");
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
  return distance;
}