#include <Arduino.h>
#include <ezButton.h>
#include "Stepper.h"
#include "Discipline.h"

const int START_STOP_LED_PIN = 5;

int rapidIntervals[] = { 2, 3, 2, 3, 2, -1 };
int mediumIntervals[] = { 3, 4, 3, -1 };
int slowIntervals[] = { 5, 4, 5, 4, 5, -1 };

ezButton button(6);
// 200 below results in a half motor rotation per toggle if the driver is configured for 400 steps per revolution - reducing this will reduce the angle of rotation per toggle
// 2500 below results in a 5000 microsecond PWM duty cycle - a longer duty cycle reults in more torque, but slower rotation
Stepper stepper(8, 7, 2500, 200);
Discipline discipline({});

void setup() {
  Serial.begin(9600);

  button.setDebounceTime(50);
  stepper.setup();

  pinMode(START_STOP_LED_PIN, OUTPUT);

  delay(100);
}

void loop() {
  button.loop();

  if (button.isPressed()) {
    if (discipline.isActive) {
      discipline.stop();
      stepper.reset();
    } else {
      discipline = Discipline(rapidIntervals);
      discipline.start();
      stepper.toggle();
      Serial.println("Discipline started");
    }
  }

  if (discipline.hasIntervalPassed()) {
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