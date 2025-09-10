#include <Arduino.h>
#include "StepperMotor.h"

const int CLOCKWISE = 0;
const int COUNTER_CLOCKWISE = 1;

void StepperMotor::setDirection(int updatedDirection) {
  direction = updatedDirection;
  digitalWrite(directionPin, direction);
}

StepperMotor::StepperMotor(int directionPin, int pulsePin, int pulseDuration, int stepsPerToggle): 
  directionPin(directionPin), pulsePin(pulsePin), pulseDuration(pulseDuration), stepsPerToggle(stepsPerToggle) {}

void StepperMotor::setup() {
  pinMode(directionPin, OUTPUT);
  pinMode(pulsePin, OUTPUT);

  setDirection(CLOCKWISE);
}

void StepperMotor::toggle() {
  if (direction == CLOCKWISE) {
    setDirection(COUNTER_CLOCKWISE);
  } else {
    setDirection(CLOCKWISE);
  }

  for (int step = 0; step < stepsPerToggle; step++) {
    digitalWrite(pulsePin, HIGH);
    delayMicroseconds(pulseDuration);
    digitalWrite(pulsePin, LOW);
    delayMicroseconds(pulseDuration);
  }
}

void Stepper::reset() {
  // move stepper to start position & clockwise
  if (direction == COUNTER_CLOCKWISE) {
    toggle();
  } 
}