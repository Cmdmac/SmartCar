#ifndef MOTOR_L298N_H_
#define MOTOR_L298N_H_
#include "Motor.h"

class Motor_L298N : public Motor {
  public:
    Motor_L298N(int pinPos, int pinNeg, int pinPwm, int stby);

}
#endif