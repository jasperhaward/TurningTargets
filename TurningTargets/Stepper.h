#ifndef Stepper_h
#define Stepper_h

class Stepper {
  private:
    int direction;
    int directionPin;
    int pulsePin;
    int pulseDuration;
    int stepsPerToggle;
    void setDirection(int direction);

  public:
    Stepper(int directionPin, int pulsePin, int pulseDuration, int stepsPerToggle);
    void setup();
    void toggle();
    void reset();
};

#endif