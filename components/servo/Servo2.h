#ifndef SERVO_H_
#define SERVO_H_
// #include <ESP32Servo.h>

class Servo2 {
  private:
    int pinPwm;
    // Servo my_servo;

  
  public:
    Servo2(int pin);
    void to(int angle);
};

#endif