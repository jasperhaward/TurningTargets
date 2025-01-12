#include <Arduino.h>
#include "Stepper.h"

const int CLOCKWISE = 0;
const int COUNTER_CLOCKWISE = 1;

void Stepper::setDirection(int direction) {
  _direction = direction;
  digitalWrite(_directionPin, direction);
}

Stepper::Stepper(int directionPin, int pulsePin, int pulseDuration, int stepsPerRevolution) {
  _directionPin = directionPin;
  _pulsePin = pulsePin;
  _pulseDuration = pulseDuration;
  _stepsPerRevolution = stepsPerRevolution;
}

void Stepper::setup() {
  pinMode(_pulsePin, OUTPUT);
  pinMode(_directionPin, OUTPUT);

  Stepper::setDirection(CLOCKWISE);
}

void Stepper::toggle() {
  if (_direction == CLOCKWISE) {
    Stepper::setDirection(COUNTER_CLOCKWISE);
  } else {
    Stepper::setDirection(CLOCKWISE);
  }

  // _stepsPerRevolution / 2 as we only want the stepper to turn 180 degrees
  for (int i = 0; i < _stepsPerRevolution / 2; i++) {
    digitalWrite(_pulsePin, HIGH);
    delayMicroseconds(_pulseDuration);
    digitalWrite(_pulsePin, LOW);
    delayMicroseconds(_pulseDuration);
  }
}

void Stepper::reset() {
  // move stepper to start position & clockwise
  if (_direction == COUNTER_CLOCKWISE) {
    Stepper::toggle();
  } 
}