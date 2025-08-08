#include <Arduino.h>
#include <SD.h>
#include <Ethernet.h>
#include "HttpController.h"

byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x0B, 0x72 };
// must be static (not DHCP) to reduce memory footprint
IPAddress ip(192, 168, 1, 178);
HttpController controller(80);

void setup() {
  Serial.begin(9600);
  controller.setup(mac, ip);
  delay(100);
}

void loop() {
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
      break;
    case RESET:
      Serial.println("RESET");
      break;
    case TOGGLE:
      Serial.println("TOGGLE");
      break;
    case NONE:
      break;
  }
}