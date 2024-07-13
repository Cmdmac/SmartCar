#include "Car.h"

Car::Car(Motor *l, Motor *r) : l(l), r(r), servo(-1) {

}

// Car::Car(int pin1Pos, int pin1Neg, int pin1Pwm, int pin2Pos, int pin2Neg, int pin2Pwm, int pinServo) : l (pin1Pos, pin1Neg, pin1Pwm), r (pin2Pos, pin2Neg, pin2Pwm), servo(pinServo) {
//   // this->servo.attach(pinServo);
// }

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
  l->speedUp();
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
  this->servo.to(angle);
}

