#include <Arduino.h>
#include <SD.h>
#include <Ethernet.h>
#ifndef EthernetController_h
#define EthernetController_h

class EthernetController {
  private:
    uint8_t* mac;
    IPAddress ip;
    EthernetServer server;
    char* extractFilename(char *path);
    char* extractContentType(char *filename);
    void writeStaticFileResponse(EthernetClient &client, char *filename);
  public:
    EthernetController(uint8_t *mac, IPAddress ip);
    void setup();
    void request();
};

#endif