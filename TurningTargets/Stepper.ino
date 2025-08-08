#include <Arduino.h>
#include "Stepper.h"

const int CLOCKWISE = 0;
const int COUNTER_CLOCKWISE = 1;

void Stepper::setDirection(int _direction) {
  direction = _direction;
  digitalWrite(directionPin, _direction);
}

Stepper::Stepper(int _directionPin, int _pulsePin, int _pulseDuration, int _stepsPerRevolution) {
  directionPin = _directionPin;
  pulsePin = _pulsePin;
  pulseDuration = _pulseDuration;
  stepsPerRevolution = _stepsPerRevolution;
}

void Stepper::setup() {
  pinMode(pulsePin, OUTPUT);
  pinMode(directionPin, OUTPUT);

  Stepper::setDirection(CLOCKWISE);
}

void Stepper::toggle() {
  if (direction == CLOCKWISE) {
    Stepper::setDirection(COUNTER_CLOCKWISE);
  } else {
    Stepper::setDirection(CLOCKWISE);
  }

  // stepsPerRevolution / 2 as we only want the stepper to turn 180 degrees
  for (int i = 0; i < stepsPerRevolution / 2; i++) {
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