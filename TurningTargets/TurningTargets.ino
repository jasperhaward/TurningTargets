#include <Arduino.h>
#include <SD.h>
#include <Ethernet.h>
#include "Stepper.h"
#include "Discipline.h"
#include "HttpController.h"

// 2500 below results in a 5000 microsecond PWM duty cycle - a longer duty cycle reults in more torque, but slower rotation
const int STEPPER_PULSE_DURATION = 2500;
// 200 below results in a half motor rotation per toggle if the driver is configured for 400 steps per revolution - reducing this will reduce the angle of rotation per toggle
const int STEPPER_STEPS_PER_TOGGLE = 200;

byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x0B, 0x72 };
// must be static (not DHCP) to reduce memory footprint
IPAddress ip(192, 168, 1, 178);
HttpController controller(80);
Stepper stepper;
Discipline discipline({});

void setup() {
  Serial.begin(9600);

  controller.setup(mac, ip);
  stepper.setup(8, 7, STEPPER_PULSE_DURATION, STEPPER_STEPS_PER_TOGGLE);

  delay(100);
}

void loop() {
  if (discipline.isActiveIntervalComplete()) {
    discipline.beginNextInterval();
    stepper.toggle();
    Serial.println("INTERVAL");
  }
  
  int intervals[20];

  ControllerAction action = controller.request(intervals, sizeof(intervals) / sizeof(intervals[0]));

  switch (action) {
    case START:
      Serial.println("START");

      for (int interval : intervals) {
        if (interval == -1) {
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
      Serial.println("RESET");
      discipline.stop();
      stepper.reset();
      break;
    case TOGGLE:
      if (!discipline.isActive) {
        Serial.println("TOGGLE");
        stepper.toggle();
      }
      break;
  }
}