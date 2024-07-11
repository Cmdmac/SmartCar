#ifndef MOTOR_H_
#define MOTOR_H_
class Motor {
  private:
  int pinPos;
  int pinNeg;
  int pinPwm;
  float speed = 500;

  // public static int SPEED_1 = 100;
  // public static int SPEED_2 = 200;
  // public static int SPEED_3 = 300;
  // public static int SPEED_4 = 400;
  // public static int SPEED_5 = 500;
  // public static int SPEED_6 = 600;
  // public static int SPEED_7 = 700;
  // public static int SPEED_8 = 800;
  // public static int SPEED_9 = 900;
  // public static int SPEED_10 = 1000;


public:
  Motor(int pinPos, int pinNeg, int pinPwm);
  void forward();
  void backward();
  void stop();
  // public void left();
  // public void right();
  void speedUp();
  void speedDown();
};

#endif