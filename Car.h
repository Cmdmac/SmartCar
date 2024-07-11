#ifndef CAR_H_
#define CAR_H_
#include "Motor.h"

class Car {
  private:
    Motor l;
    Motor r;



  public:
    Car(int pin1Pos, int pin1Neg, int pin1Pwm, int pin2Pos, int pin2Neg, int pin2Pwm);

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

};

#endif