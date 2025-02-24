#include "Car.h"
#include <Arduino.h>
#include "src/common/Command.h"

Car::Car(Motor *l, Motor *r, /*Servo2 *s,*/ UltraSound *us) : l(l), r(r), /*s(s),*/ us(us), directValue(0), speed(0) {

}

// Car::Car(int pin1Pos, int pin1Neg, int pin1Pwm, int pin2Pos, int pin2Neg, int pin2Pwm, int pinServo) : l (pin1Pos, pin1Neg, pin1Pwm), r (pin2Pos, pin2Neg, pin2Pwm), servo(pinServo) {
//   // this->servo.attach(pinServo);
// }

void Car::parse(char* data, int len) {
  int command = data[0];
  switch(command) {
    case CMD_FORWARD:
      forward();
      break;

    case CMD_BACKWARD:
      backward();
      break;

    case CMD_TURN_LEFT:
      left();
      break;

    case CMD_TURN_RIGHT:
      right();
      break;

    case CMD_BRAKE:
      stop();
      break;
  }
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
  this->speed = speed;
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
  // this->s->to(angle);
}

void Car::getDistance() {
  this->us->getDistance();
}

void Car::direct(float d) {
  this->directValue = d;
  if (d < 0) {
    
  } else if (d > 0) {

  } else {
    
  }
  
}

void Car::drive(float speed) {
  this->speed = speed;
  delay(1);
  if (speed == 0) {
    l->stop();
    r->stop();
    l->setSpeed(0);
    r->setSpeed(0);
  } else if (speed > 0.2) {
    float speedLeft = speed;
    float speedRight = speed;
    if (this->directValue > 0) {
      speedLeft = (1 - this->directValue) * speedLeft;
    } else if (this->directValue < 0) {
      speedRight = (1 - (-this->directValue)) * speedRight;
    }
    l->setSpeed(speedLeft);
    r->setSpeed(speedRight);
    l->forward();
    r->forward();
  } else if (speed < -0.2) {
    float speedLeft = -speed;
    float speedRight = -speed;
    if (this->directValue > 0) {
      speedLeft = (1 - this->directValue) * speedLeft;
    } else if (this->directValue < 0)  {
      speedRight = (1 - (-this->directValue)) * speedRight;
    }
    l->setSpeed(speedLeft);
    r->setSpeed(speedRight);
    l->backward();
    r->backward();
  }  
}
