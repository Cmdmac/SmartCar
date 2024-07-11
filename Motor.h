#ifndef MOTOR_H_
#define MOTOR_H_
class Motor {
  private int pinPos;
  private int pinNeg;
  private int pinPwm;
  
  public int SPEED_1 = 100;
  public int SPEED_2 = 200;
  public int SPEED_3 = 300;
  public int SPEED_4 = 400;
  public int SPEED_5 = 500;
  public int SPEED_6 = 600;
  public int SPEED_7 = 700;
  public int SPEED_8 = 800;
  public int SPEED_9 = 900;
  public int SPEED_10 = 1000;

  private speed = SPEED_5;

  public Motor(int pinPos, int pinNeg, int pinPwm);
  public void forward();
  public void backward();
  public void stop();
  // public void left();
  // public void right();
  public void speedUp();
  public void speedDown();
}

#endif