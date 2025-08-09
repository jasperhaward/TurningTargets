#include <Arduino.h>
#include <SD.h>
#include <Ethernet.h>
#ifndef HttpController_h
#define HttpController_h

enum ControllerAction {
  NONE,
  START,
  RESET,
  TOGGLE
};

class HttpController {
  private:
    EthernetServer server;
    char* extractFilename(char *path);
    char* extractQueryParameter(char *path, char *key);
    void writeContentTypeHeadersToResponse(EthernetClient &client, char *filename);
    void writeFileToResponse(EthernetClient &client, char *filename);
    bool writeRequestToFile(EthernetClient &client, char *filename);
    bool parseIntervals(char *value, int *intervals, size_t length);
  public:
    HttpController(uint8_t port);
    void setup(uint8_t *mac);
    ControllerAction request(int *intervals, size_t length);
};

#endif