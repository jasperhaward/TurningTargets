#ifndef StepperMotor_h
#define StepperMotor_h

class StepperMotor {
  private:
    int pulsePin;
    int directionPin;
    int direction;
    int pulseDuration;
    int stepsPerToggle;
    void setDirection(int direction);

  public:
    StepperMotor(int directionPin, int pulsePin, int pulseDuration, int stepsPerToggle);
    void setup();
    void toggle();
    void reset();
};

#endif