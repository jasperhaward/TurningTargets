#include <Arduino.h>
#include <SD.h>
#include <Ethernet.h>
#include "StepperMotor.h"
#include "Discipline.h"
#include "HttpController.h"

extern const int DISCIPLINE_TERMINATOR;

byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x0B, 0x72 };
HttpController controller(80);
// 3000 = 6000 microsecond PWM duty cycle - a longer duty cycle reults in more torque, but slower rotation
// 100 =  45deg/quarter motor rotation per toggle if the driver is configured for 400 steps per revolution - reducing this will reduce the angle of rotation per toggle
StepperMotor stepper(8, 7, 3000, 100);
Discipline discipline({});

void setup() {
  Serial.begin(9600);

  controller.setup(mac);
  stepper.setup();

  delay(100);
}

void loop() {
  if (discipline.isActiveIntervalComplete()) {
    discipline.beginNextInterval();
    stepper.toggle();
    Serial.println(F("INTERVAL"));
  }
  
  int intervals[20];

  ControllerAction action = controller.request(intervals, sizeof(intervals) / sizeof(intervals[0]));

  switch (action) {
    case START:
      Serial.println(F("START"));

      for (int interval : intervals) {
        if (interval == DISCIPLINE_TERMINATOR) {
          break;
        }
        Serial.println(interval);
      }

      if (discipline.isActive) {
        discipline.stop();
      }

      discipline = Discipline(intervals);
      stepper.reset();
      discipline.start();
      stepper.toggle();
      break;
    case RESET:
      Serial.println(F("RESET"));
      discipline.stop();
      stepper.reset();
      break;
    case TOGGLE:
      if (!discipline.isActive) {
        Serial.println(F("TOGGLE"));
        stepper.toggle();
      }
      break;
  }
}