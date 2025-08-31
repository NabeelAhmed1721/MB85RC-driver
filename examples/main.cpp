#include <Arduino.h>
#include "MB85RC.h"

void hexdump(uint8_t* buffer, size_t length);

MB85RC fram = MB85RC();

void setup() {
  Serial.begin(9600);

  // write text buffer at address 0x0100
  const char* text = "Nabeel was here!";
  fram.write(0x0100, (uint8_t*)text, strlen(text));


  // read 1024 bytes from address 0x0000
  const size_t SIZE = 1024; 
  uint8_t buffer[SIZE];
  fram.read(0x0000, buffer, SIZE);
  hexdump(buffer, SIZE);

  // close the driver
  fram.close();
}

void loop() {}

void hexdump(const uint8_t* buffer, size_t length) {
  const size_t bytesPerLine = 16; 
  char line[80];

  for (size_t i = 0; i < length; i += bytesPerLine) {
    char* p = line;

    sprintf(p, "%04X: ", (unsigned int)i);
    p += strlen(p);

    for (size_t j = 0; j < bytesPerLine; j++) {
      if (i + j < length) {
        sprintf(p, "%02X ", buffer[i + j]);
      } else {
        sprintf(p, "   ");
      }
      p += 3;
    }

    *p++ = ' ';

    for (size_t j = 0; j < bytesPerLine; j++) {
      if (i + j < length) {
        char c = buffer[i + j];
        *p++ = (c >= 32 && c <= 126) ? c : '.';
      }
    }

    // null-terminate 
    *p = '\0';

    Serial.println(line);
  }
}