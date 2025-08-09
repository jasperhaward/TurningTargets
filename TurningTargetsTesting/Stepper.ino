#include <Arduino.h>
#include "Stepper.h"

const int CLOCKWISE = 0;
const int COUNTER_CLOCKWISE = 1;

void Stepper::setDirection(int _direction) {
  direction = _direction;
  digitalWrite(directionPin, direction);
}

Stepper::Stepper() {}

void Stepper::setup(int _directionPin, int _pulsePin, int _pulseDuration, int _stepsPerToggle) {
  directionPin = _directionPin;
  pulsePin = _pulsePin;
  pulseDuration = _pulseDuration;
  stepsPerToggle = _stepsPerToggle;

  pinMode(directionPin, OUTPUT);
  pinMode(pulsePin, OUTPUT);

  Stepper::setDirection(CLOCKWISE);
}

void Stepper::toggle() {
  if (direction == CLOCKWISE) {
    Stepper::setDirection(COUNTER_CLOCKWISE);
  } else {
    Stepper::setDirection(CLOCKWISE);
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
    Stepper::toggle();
  } 
}