#include <Ethernet.h>
#include "HttpController.h"

const char CONFIG_FILE[] = "CONFIG";

// converts path into Arduino SD compatible filename eg: INDEX.CSS from /index.css
char* HttpController::extractFilename(char *path) {
  // remove prefixed /
  char* filename = path + 1;

  // uppercase remaining path
  for (uint8_t count = 0; count < strlen(filename); count++) {
    filename[count] = toupper(filename[count]);
  }
  
  return filename;
}

// key must be suffixed with = eg `intervals=`
char* HttpController::extractQueryParameter(char *path, char *key) {
  return strstr(path, key) + strlen(key);
}

char* HttpController::extractContentType(char *filename) {
  // special case for CONFIG file -> Content-Type: application/json
  if (strstr(filename, CONFIG_FILE)) {
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

void HttpController::writeFileToResponse(EthernetClient &client, char *filename) {
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

bool HttpController::writeRequestToFile(EthernetClient &client, char *filename) {
  if (SD.exists(filename)) {
    if (!SD.remove(filename)) {
      return false;
    }
  }

  File file = SD.open(filename, FILE_WRITE);

  if (!file) {
    return false;
  }

  while (client.available()) {
    char c = client.read();
    file.print(c);
  }

  file.close();
  
  return true;
}

bool HttpController::parseIntervals(char *value, int *intervals, size_t length) {
  char *pointer;
  int index = 0;
  
  pointer = strtok(value, ",");

  if (pointer == NULL) {
    return false;
  }

  while (pointer != NULL) {
    intervals[index] = atoi(pointer); 
    pointer = strtok(NULL, ",");
    index++;

    // prevent overflow from more intervals being provided than can fit in `*intervals`
    if (index > length - 1) {
      return false;
    }
  }
  
  intervals[index] = -1; // -1 is termination character in `int *intervals`

  return true;
}

HttpController::HttpController(uint16_t port) : server(port) {}

void HttpController::setup(uint8_t *_mac, IPAddress ip) {
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

ControllerAction HttpController::request(int *intervals, size_t length) {
  EthernetClient client = server.available();
  ControllerAction action = NONE;

  // check client is connected & read upto first space in HTTP headers: 'GET /api/config HTTP/1.1' -> '/api/config HTTP/1.1'
  if (client && client.connected() && client.find(' ')) {
    char path[64];
    int end = client.readBytesUntil(' ', path, sizeof(path)); // read into path until second space eg : '/api/config HTTP/1.1' -> '/api/config'
    path[end] = '\0';

    bool currentLineIsBlank = true;

    while (client.available()) {
      char character = client.read();
      
      if (character == '\n' && currentLineIsBlank) {
        // if reached end of line (a newline character) and the line is blank, the HTTP headers have ended,
        // break out of while loop so we can access HTTP payload later
        break;
      } else if (character == '\n') {
        currentLineIsBlank = true; // start of new line
      } else if (character != '\r') {
        currentLineIsBlank = false;  // recieved character so line is not blank
      }
    }

    // handle explicit 'API' routes
    if (strstr(path, "/api")) {
      bool isSuccess = true;

      if (strstr(path, "/start?intervals=")) {
        char* intervalsParameter = extractQueryParameter(path, "intervals=");

        if (parseIntervals(intervalsParameter, intervals, length)) {
          action = START;
        } else {
          isSuccess = false;
        }
      } else if (strstr(path, "/stop")) {
        action = STOP;
      } else if (strstr(path, "/toggle")) {
        action = TOGGLE;
      } else if (strstr(path, "/save")) {
        isSuccess = writeRequestToFile(client, CONFIG_FILE);
      } 

      if (isSuccess) {
        client.println("HTTP/1.1 204 No Content");
      } else {
        client.println("HTTP/1.1 500 Internal Server Error");
      }

      client.println("Connection: close");
      client.println();
    } else {
      // handle static files
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
        writeFileToResponse(client, filename);
      } else {
        client.println("HTTP/1.1 404 Not Found");
        client.println("Connection: close");
        client.println();
      }

      client.stop();
    }
  }

  return action;
}