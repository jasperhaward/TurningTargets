#include <Arduino.h>
#include <ezButton.h>
#include "Stepper.h"
#include "Discipline.h"

const int START_STOP_LED_PIN = 5;

int rapidIntervals[] = { 2, 3, 2, 3, 2, -1 };
int mediumIntervals[] = { 3, 4, 3, -1 };
int slowIntervals[] = { 5, 4, 5, 4, 5, -1 };

ezButton button(6);
Stepper stepper(8, 7, 800, 400);
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
    }
  }

  if (discipline.hasIntervalPassed()) {
    discipline.beginNextInterval();
    stepper.toggle();
  }

  if (discipline.isActive) {
    digitalWrite(START_STOP_LED_PIN, HIGH);
  } else {
    digitalWrite(START_STOP_LED_PIN, LOW);
  }  
}