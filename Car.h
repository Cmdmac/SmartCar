#ifndef CAR_H_
#define CAR_H_
#include "Motor.h"
#include "Servo.h"

class Car {
  private:
    Motor l;
    Motor r;
    Servo servo;


  public:
    Car(int pin1Pos, int pin1Neg, int pin1Pwm, int pin2Pos, int pin2Neg, int pin2Pwm, int pinServo);

    void forward();
    void backward();
    void stop();
    // public void left();
    // public void right();
    void speedUp();
    void speedDown();
    void setSpeed(float speed);
      
    void left();
    void right();

    void faceTo(int angle);

};

#endif