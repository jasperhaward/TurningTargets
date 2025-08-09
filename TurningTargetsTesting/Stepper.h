#ifndef Stepper_h
#define Stepper_h

class Stepper {
  private:
    int pulsePin;
    int directionPin;
    int direction;
    int pulseDuration;
    int stepsPerToggle;
    void setDirection(int direction);

  public:
    Stepper();
    void setup(int directionPin, int pulsePin, int pulseDuration, int stepsPerToggle);
    void toggle();
    void reset();
};

#endif