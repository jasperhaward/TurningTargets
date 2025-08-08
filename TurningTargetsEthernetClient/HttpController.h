#include <Arduino.h>
#include <SD.h>
#include <Ethernet.h>
#ifndef HttpController_h
#define HttpController_h

enum ControllerAction {
  NONE,
  START,
  STOP,
  TOGGLE
};

class HttpController {
  private:
    uint8_t* mac;
    IPAddress ip;
    EthernetServer server;
    char* extractFilename(char *path);
    char* extractQueryParameter(char *path, char *key);
    char* extractContentType(char *filename);
    void writeFileToResponse(EthernetClient &client, char *filename);
    bool writeRequestToFile(EthernetClient &client, char *filename);
    bool parseIntervals(char *value, int *intervals, size_t length);
  public:
    HttpController(uint16_t port);
    void setup(uint8_t *mac, IPAddress ip);
    ControllerAction request(int *intervals, size_t length);
};

#endif