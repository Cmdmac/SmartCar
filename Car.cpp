#include "Car.h"
#include <Arduino.h>

Car::Car(Motor *l, Motor *r, Servo *s, UltraSound *us) : l(l), r(r), s(s), us(us) {

}

// Car::Car(int pin1Pos, int pin1Neg, int pin1Pwm, int pin2Pos, int pin2Neg, int pin2Pwm, int pinServo) : l (pin1Pos, pin1Neg, pin1Pwm), r (pin2Pos, pin2Neg, pin2Pwm), servo(pinServo) {
//   // this->servo.attach(pinServo);
// }

void Car::setSerialChanelWithHub(SoftwareSerial* serial) {
  this->serialChanelWithHub = serial;
}

  char data[128] = {0};
  // memset(data, 0, 128);
  int count = 0;

void Car::waitForHubMessages() {
  if (this->serialChanelWithHub == NULL) {
    return;
  }


  while(this->serialChanelWithHub->available() > 0) {
    int c = this->serialChanelWithHub->read();
    data[count++] = c;
    if (c == '\n') {
      Serial.print(data);
      memset(data, 0, 128);
      count = 0;
      break;
    }
  }
  // Serial.println(count);
  // Serial.println(data);

}

void Car::forward() {
  l->forward();
  r->forward();
}

void Car::backward() {
  l->backward();
  r->backward();
}

void Car::stop() {
  l->stop();
  r->stop();
}
    // public void left();
    // public void right();
void Car::speedUp() {
  l->speedUp();
  r->speedUp();
}
void Car::speedDown() {
  l->speedDown();
  r->speedDown();
}

void Car::setSpeed(float speed) {
  l->setSpeed(speed);
  r->setSpeed(speed);
}

void Car::left() {
  l->setSpeed(0.2);
  r->setSpeed(0.5);
}

void Car::right() {
  r->setSpeed(0.2);
  l->setSpeed(0.5);
}

void Car::faceTo(int angle) {
  this->s->to(angle);
}

void Car::getDistance() {
  this->us->getDistance();
}

