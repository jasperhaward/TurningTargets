#ifndef Stepper_h
#define Stepper_h

class Stepper {
  private:
    int _direction;
    int _directionPin;
    int _pulsePin;
    int _pulseDuration;
    int _stepsPerRevolution;
    void setDirection(int direction);

  public:
    Stepper(int directionPin, int pulsePin, int pulseDuration, int stepsPerRevolution);
    void setup();
    void toggle();
    void reset();
};

#endif