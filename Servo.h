#ifndef SERVO_H_
#define SERVO_H_

class Servo {
  private:
    int pinPwm;
  
  public:
    Servo(int pin);
    void to(int angle);
};

#endif