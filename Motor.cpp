#include<Arduino.h>
#include"Motor.h"


Motor::Motor(int pinPos, int pinNeg, int pinPwm) {
  this->pinPos = pinPos;
  this->pinNeg = pinNeg;
  this->pinPwm = pinPwm;
  this->speed = 0.5;
  pinMode(this->pinPos, OUTPUT);
  pinMode(this->pinNeg, OUTPUT);
  pinMode(this->pinPwm, OUTPUT);
}


void Motor::forward() {
  Serial.println("Motor forward");

}

void Motor::backward() {
 
}

void Motor::stop() {

}

// void Motor::forward() {
//   Serial.println("forward");
//   Serial.println(this->speed);
//   digitalWrite(this->pinPos, HIGH);
//   digitalWrite(this->pinNeg, LOW);
//   analogWrite(this->pinPwm, this->speed  * 255);  

// }

// void Motor::backward() {
//   digitalWrite(this->pinPos, LOW);
//   digitalWrite(this->pinNeg, HIGH);
// }

// void Motor::stop() {
//     Serial.println("stop");
//   digitalWrite(this->pinPos, LOW);
//   digitalWrite(this->pinNeg, LOW);
//   analogWrite(this->pinPwm, 0);  
// }


// public void Motor::left();
// public void Motor::right();
void Motor::speedUp() {
  this->speed = this->speed + 0.1;
  if (this->speed > 1) {
    this->speed = 1;
  }
  Serial.println("speedUp");
  Serial.println(this->speed);
  analogWrite(this->pinPwm, this->speed * 255);  
}

void Motor::speedDown() {
  this->speed = this->speed - 0.1;
  if (this->speed < 0.2) {
    this->speed = 0.2;
  }
  Serial.println("speedDown");
    Serial.println(this->speed);
  analogWrite(this->pinPwm, this->speed * 255);  
}

void Motor::setSpeed(float speed) {
  analogWrite(this->pinPwm, this->speed * 255);  
}
