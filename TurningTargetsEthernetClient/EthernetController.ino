#include <Ethernet.h>
#include "EthernetController.h"

// converts path into Arduino SD compatible filename eg: INDEX.CSS from /index.css
char* EthernetController::extractFilename(char *path) {
  // remove prefixed /
  char* filename = path + 1;

  // uppercase remaining path
  for (uint8_t count = 0; count < strlen(filename); count++) {
    filename[count] = toupper(filename[count]);
  }
  
  return filename;
}

char* EthernetController::extractContentType(char *filename) {
  // special case for CONFIG file -> Content-Type: application/json
  if (strstr(filename, "CONFIG")) {
    return "Content-Type: application/json";
  } else if (strstr(filename, ".JS")) {
    return "Content-Type: application/javascript";
  } else if (strstr(filename, ".CSS")) {
    return "Content-Type: text/css";
  } else if (strstr(filename, ".PNG")) {
    return "Content-Type: image/png";
  } else if (strstr(filename, ".HTM")) {
    return "Content-Type: text/html";
  } else {
    return "Content-Type: text/plain";
  }
}

void EthernetController::writeStaticFileResponse(EthernetClient &client, char *filename) {
  File file = SD.open(filename);

  byte clientBuffer[64];
  int byteCount = 0;

  while (file.available()) {
    clientBuffer[byteCount] = file.read();
    byteCount++;

    if (byteCount > 63) {
      client.write(clientBuffer, 64);
      byteCount = 0;
    }
  }

  // final remainder < 64 byte cleanup packet
  if (byteCount > 0) {
    client.write(clientBuffer, byteCount);
  }

  file.close();
}


EthernetController::EthernetController(uint8_t *_mac, IPAddress _ip) : server(80) {
  mac = _mac;
  ip = _ip;
}

void EthernetController::setup() {
  Ethernet.begin(mac, ip);
  Ethernet.init(10);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield/hardware not found.");
    while (true);
  }

  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
    while (true);
  }

  if (!SD.begin(4)) {
    Serial.println("SD card not found.");
    while (true);
  }

  server.begin();
}

// handles request based on first line of HTTP headers eg: 'GET /api/config HTTP/1.1'
void EthernetController::request() {
  EthernetClient client = server.available();

  // check client is connected & read upto first space in HTTP headers eg: '/api/config HTTP/1.1'
  if (client && client.connected() && client.find(' ')) {
    char path[20];
    // read bytes into path until second space in HTTP headers eg : path -> '/api/config'
    int end = client.readBytesUntil(' ', path, sizeof(path));
    path[end] = '\0';
    
    while (client.read() != -1);

    // handle explicit 'API' routes
    if (strstr(path, "/toggle") || strstr(path, "/start") || strstr(path, "/save")) {
      if (strstr(path, "/toggle")) {
        Serial.println("Toggle called");
      } else if (strstr(path, "/start")) {
        // TODO: parse intervals
        Serial.println("Start called");
      } else if (strstr(path, "/save")) {
        // TODO: save updated config JSON
        Serial.println("Save called");
      } 
    
      client.println("HTTP/1.1 204 No Content");
      client.println("Connection: close");
      client.println();
    } else {
      // handle loading static files
      char* filename;

      // handle index / route
      if (strstr(path, "/") && strlen(path) == 1) {
        filename = "INDEX.HTM";
      } else {
        filename = extractFilename(path);
      }

      if (SD.exists(filename)) {
        client.println("HTTP/1.1 200 OK");
        client.println(extractContentType(filename));
        client.println("Connection: close");
        client.println();
        writeStaticFileResponse(client, filename);
      } else {
        client.println("HTTP/1.1 404 Not Found");
      }
    }

    client.stop();
  }
}