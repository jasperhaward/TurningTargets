#include <Arduino.h>
#include "Stepper.h"
#include "Discipline.h"

const int START_STOP_BUTTON_PIN = 12;
const int START_STOP_LED_PIN = 13;

const int STEPPER_DIRECTION_PIN = 8;
const int STEPPER_PULSE_PIN = 7;
// 2500 below results in a 5000 microsecond PWM duty cycle - a longer duty cycle reults in more torque, but slower rotation
const int STEPPER_PULSE_DURATION = 2500;
// 200 below results in a half motor rotation per toggle if the driver is configured for 400 steps per revolution - reducing this will reduce the angle of rotation per toggle
const int STEPPER_STEPS_PER_TOGGLE = 200;

int rapidIntervals[] = { 2, 3, 2, 3, 2, -1 };
int mediumIntervals[] = { 3, 4, 3, -1 };
int slowIntervals[] = { 5, 4, 5, 4, 5, -1 };

Stepper stepper;
Discipline discipline({});

void setup() {
  Serial.begin(9600);

  stepper.setup(
    STEPPER_DIRECTION_PIN, 
    STEPPER_PULSE_PIN,
    STEPPER_PULSE_DURATION,
    STEPPER_STEPS_PER_TOGGLE
  );

  pinMode(START_STOP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(START_STOP_LED_PIN, OUTPUT);

  delay(100);
}

void loop() {
  int buttonState = digitalRead(START_STOP_BUTTON_PIN);

  // pull-up resistor means the button logic is inverted, HIGH when open, and LOW when pressed
  if (buttonState == LOW) {
    if (discipline.isActive) {
      discipline.stop();
      stepper.reset();
      Serial.println("Discipline stopped");
    } else {
      discipline = Discipline(rapidIntervals);
      discipline.start();
      stepper.toggle();
      Serial.println("Discipline started");
    }
  }

  if (discipline.isActiveIntervalComplete()) {
    discipline.beginNextInterval();
    stepper.toggle();
    Serial.println("Interval passed");
  }

  if (discipline.isActive) {
    digitalWrite(START_STOP_LED_PIN, HIGH);
  } else {
    digitalWrite(START_STOP_LED_PIN, LOW);
  }
}