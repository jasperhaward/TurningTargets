#include <Arduino.h>
#include <SD.h>
#include <Ethernet.h>
#include <ezButton.h>
#include "Stepper.h"
#include "Discipline.h"

byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x0B, 0x72 };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);

const int START_STOP_LED_PIN = 5;

int rapidIntervals[] = { 2, 3, 2, 3, 2, -1 };
int mediumIntervals[] = { 3, 4, 3, -1 };
int slowIntervals[] = { 5, 4, 5, 4, 5, -1 };

char *token;
const char *delimiter = ",";

ezButton button(6);
Stepper stepper(8, 7, 800, 400);
Discipline discipline({});

void setup() {
  Serial.begin(9600);

  Ethernet.begin(mac, ip);
  Ethernet.init(10);

  File file = SD.open("CONFIG");

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield/hardware not found.");
    while (true)
      ;
  }

  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
    while (true)
      ;
  }

  if (!SD.begin(4)) {
    Serial.println("SD card not found.");
    while (true);
  }

  server.begin();

  button.setDebounceTime(50);
  stepper.setup();

  pinMode(START_STOP_LED_PIN, OUTPUT);

  delay(100);
}

void loop() {
  button.loop();

  EthernetClient client = server.available();

  if (client) {
    if (client.connected()) {
      if (client.find(' ')) {  // GET /fn HTTP/1.1
        char fn[20];
        int l = client.readBytesUntil(' ', fn, sizeof(fn));
        fn[l] = 0;
        while (client.read() != -1);

        if (strstr(fn, "/toggle")) {
          stepper.toggle();
        } else if (strstr(fn, "/start/")) {
          char * pos;
          pos = strtok (str, "/");
          pos = strtok (NULL, "/");
            Serial.println(pos);
          token = strtok(&fn[pos];, delimiter);

          while (token != NULL) {
            Serial.println(token);
            token = strtok(NULL, delimiter);
          }

          stepper.reset();
          discipline = Discipline(rapidIntervals);
          discipline.start();
          stepper.toggle();
        }

        // send a standard http response header
        client.println("HTTP/1.1 200 OK");

        char filename[20];

        if (strstr(fn, "/config")) {
          client.println("Content-Type: application/json");
          strcpy(filename, "CONFIG");
        } else if (strstr(fn, "/index.js")) {
          client.println("Content-Type: application/javascript");
          strcpy(filename, "INDEX.JS");
        } else if (strstr(fn, "/index.css")) {
          client.println("Content-Type: text/css");
          strcpy(filename, "INDEX.CSS");
        } else if (strstr(fn, "/logo.png")) {
          client.println("Content-Type: image/png");
          strcpy(filename, "LOGO.PNG");
        } else {
          client.println("Content-Type: text/html");
          strcpy(filename, "INDEX.HTM");
        }

        client.println("Connection: close");
        client.println();

        File file = SD.open(filename);

        if (file) {
          byte clientBuf[64];
          int clientCount = 0;

          while (file.available()) {
            clientBuf[clientCount] = file.read();
            clientCount++;

            if (clientCount > 63) {
              client.write(clientBuf, 64);
              clientCount = 0;
            }
          }
          //final <64 byte cleanup packet
          if (clientCount > 0) client.write(clientBuf, clientCount);
          // close the file:
          file.close();
        }

        client.stop();
      } 
    }
  }

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