#include <Arduino.h>
#include <SD.h>
#include <Ethernet.h>
#include "EthernetController.h"

byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x0B, 0x72 };
IPAddress ip(192, 168, 1, 178);
EthernetController controller(mac, ip);

void setup() {
  Serial.begin(9600);
  controller.setup();
  delay(100);
}

void loop() {
  controller.request();
}