#include "DS18B20.h"
#include <Arduino.h>

DS18B20::DS18B20(uint8_t pin) : oneWire(pin) {}

bool DS18B20::begin() {
    if (!readAddress()) {
        return false; // Sensor not found
    }
    return true;
}

bool DS18B20::readAddress() {
    if (!oneWire.search(address)) {
        oneWire.reset_search();
        return false;
    }
    if (OneWire::crc8(address, 7) != address[7]) {
        return false; // Invalid CRC check for the address
    }
    return (address[0] == 0x28); // Check if it is DS18B20
}

void DS18B20::startConversion() {
    oneWire.reset();
    oneWire.select(address);
    oneWire.write(0x44, 1); // Start temperature conversion with strong pull-up
}

float DS18B20::readScratchpad() {
    oneWire.reset();
    oneWire.select(address);
    oneWire.write(0xBE); // Read scratchpad command

    uint8_t data[9];
    for (int i = 0; i < 9; i++) {
        data[i] = oneWire.read();
    }

    if (OneWire::crc8(data, 8) != data[8]) {
        return -127.0; // Return error if CRC is invalid
    }

    int16_t rawTemperature = (data[1] << 8) | data[0]; // Combine LSB and MSB
    return rawTemperature * 0.0625; // Convert to Celsius (each bit is 0.0625 °C)
}

float DS18B20::getTemperatureC() {
    startConversion();
    delay(750); // Wait for conversion to complete (750ms for 12-bit resolution)
    return readScratchpad();
}