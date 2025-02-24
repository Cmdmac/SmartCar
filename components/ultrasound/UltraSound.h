#ifndef ULTRASOUND_H_
#define ULTRASOUND_H_


class UltraSound {
  private:
    int pinTri;
    int pinEco;

  public:
    UltraSound(int pinTri, int pinEco);
    int getDistance();
};
#endif