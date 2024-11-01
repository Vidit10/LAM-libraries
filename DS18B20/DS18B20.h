#ifndef DS18B20_H
#define DS18B20_H

#include <OneWire.h>

class DS18B20 {
public:
    DS18B20(uint8_t pin);               // Constructor that takes the pin where DS18B20 is connected
    bool begin();                       // Initializes the sensor
    float getTemperatureC();            // Returns temperature in Celsius
private:
    OneWire oneWire;                    // OneWire object for DS18B20 communication
    uint8_t address[8];                 // Array to store the unique address of the sensor
    bool readAddress();                 // Reads the address of the DS18B20
    void startConversion();             // Starts temperature conversion
    float readScratchpad();             // Reads temperature from the scratchpad
};

#endif // DS18B20_H