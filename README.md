# MB85RC FRAM AVR Driver

C++ driver for Fujitsu MB85RC FRAM device on AVR. FRAM (Ferroelectric RAM) is an incredibly fast and resilent non-volatile memory that can be used to store data for a long time.

## Requirements
- Fujitsu MB85RC series FRAM chip
- AVR Microcontroller (e.g. ATmega328P)
- Arduino Wire library (`<Wire.h>`)
- AVR toolchain


## Features
- Supports MB85RC I2C FRAM devices (up to 8 on one bus)
- Single-byte and multi-byte read/write
- 32-byte chunking for Wire buffer

## Wiring
Typical for MB85RC256V or similar:

- VCC → 3.3V or 5V
- GND → GND
- SDA → Master SDA
- SCL → Master SCL
- WP  → GND (write protect; pull to VCC to disable writes)
- A0–A2 → Address pins (set device address extension)

> NOTE: The MB85RC uses the address `0x50` by default, but can be extended from `0x50` to `0x57` by setting the address extension pins. This allows up to 8 devices to be connected to the same I2C bus.

## Usage
```cpp
#include "MB85RC.h"

// MB85RC fram(3); // set address extension to 3 (0x53)
MB85RC fram; // default 0x50

void setup() {
  Serial.begin(9600);

  const char* msg = "Hello FRAM!";
  fram.write(0x0100, (uint8_t*)msg, strlen(msg));

  uint8_t buffer[64];
  fram.read(0x0100, buffer, strlen(msg));

  for (size_t i = 0; i < strlen(msg); i++) {
    Serial.print((char)buffer[i]);
  }
}

void loop() {}
```

## Example Output

```bash
0000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
0010: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
0020: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
...
0100: 48 65 6C 6C 6F 20 46 52 41 4D 21 00 00 00 00 00  Hello FRAM!.....
0110: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
0120: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
```
