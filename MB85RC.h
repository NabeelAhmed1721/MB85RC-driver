#include <stdint.h>

/**
 * MB85RC FRAM driver
 * NOTE: Only one instance of this class should be created
 * as it manages the global I2C Wire interface
 */
class MB85RC {
private:
	static constexpr uint8_t BASE_ADDRESS = 0x50;
	uint8_t device_address = MB85RC::BASE_ADDRESS;
	static bool initialized;
	// can fail if address is not in range
	bool writeAddress(uint16_t address, bool end = true);
public:
	MB85RC(uint8_t address_extension = 0);
	void close();

	// single byte operations
	uint8_t readByte(uint16_t address);
	bool writeByte(uint16_t address, uint8_t value);

	// multi-byte operations
	bool read(uint16_t address, uint8_t* buffer, size_t length);
	bool write(uint16_t address, const uint8_t* buffer, size_t length);

	// utility functions
	uint8_t getAddress() const;
};