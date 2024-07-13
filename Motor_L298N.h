#ifndef MOTOR_L298N_H_
#define MOTOR_L298N_H_
#include "Motor.h"

class Motor_L298N : public Motor {
  private:
    int pinStby;

  public:
    Motor_L298N(int pinPos, int pinNeg, int pinPwm, int pinStby);

    void forward();
    void backward();
    void stop();
};
#endif