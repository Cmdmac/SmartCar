#ifndef MOTOR_TB6612FNG_H_
#define MOTOR_TB6612FNG_H_
#include "Motor.h"

class Motor_TB6612FNG : public Motor {
  private:
    int pinStby;

  public:
    Motor_TB6612FNG(int pinPos, int pinNeg, int pinPwm, int pinStby);
    void forward();
    void backward();
    void stop();
};

#endif