#include <Wire.h>

#include "MB85RC.h"

// driver code for MB85RC
#define min(a, b)((a) < (b) ? (a) : (b))

bool MB85RC::initialized = false;

/**
 * Initializes the MB85RC driver with a given device address
 * Up to 8 devices MB85RC devices can be connected to the same I2C bus
 */
MB85RC::MB85RC(uint8_t address_extension)
    // uses lower 3 bits (up to 8 devices)
	: device_address(MB85RC::BASE_ADDRESS | (address_extension & 0x07)) {
	if (MB85RC::initialized) return;
	MB85RC::initialized = true;
	Wire.begin();
}

void MB85RC::close() {
	if (!MB85RC::initialized) return;
	MB85RC::initialized = false;
	Wire.end();
}

bool MB85RC::writeAddress(uint16_t address, bool end) {
	Wire.beginTransmission(this -> device_address);
	Wire.write((address >> 8) & 0xFF);
	Wire.write(address & 0xFF);
	if (end) {
		return Wire.endTransmission() == 0;
	}
	return true;
}

/**
 * Reads a byte from the specified address
 */
uint8_t MB85RC::readByte(uint16_t address) {
	if (!this -> writeAddress(address, true)) return 0;
	Wire.requestFrom(this -> device_address, (uint8_t) 1);
	if (!Wire.available()) return 0;
	return Wire.read();
}

/**
 * Writes a byte to the specified address
 */
bool MB85RC::writeByte(uint16_t address, uint8_t value) {
	if (!this -> writeAddress(address, false)) return false; // address and write data are in one transmission
	Wire.write(value);
	return Wire.endTransmission() == 0;
}

bool MB85RC::read(uint16_t address, uint8_t * buffer, size_t length) {
	if (!buffer || length == 0) return false;

	// requestFrom can only read up to 32 bytes at a time
	while (length > 0) {
		uint8_t chunk = min(length, BUFFER_LENGTH);
		if (!this -> writeAddress(address, true)) return false;

		// check if read was successful
		if (Wire.requestFrom(this -> device_address, chunk) != chunk)
			return false;

		for (uint8_t i = 0; i < chunk; i++) {
			* buffer++ = Wire.read();
		}

		length -= chunk;
		address += chunk;
	}

	return true;
}

bool MB85RC::write(uint16_t address, const uint8_t * buffer, size_t length) {
	if (!buffer || length == 0) return false;

	// write can only write up to 32 bytes at a time
	while (length > 0) {
		uint8_t chunk = min(length, BUFFER_LENGTH);
		if (!this -> writeAddress(address, false)) return false;
		Wire.write(buffer, chunk);
		if (Wire.endTransmission() != 0) return false;
		length -= chunk;
		buffer += chunk;
		address += chunk;
	}

	return true;
}

uint8_t MB85RC::getAddress() const {
	return this -> device_address;
}
