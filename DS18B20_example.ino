#include "DS18B20.h"

// Pin connected to the DS18B20 data pin
#define DS18B20_PIN 14

DS18B20 sensor(DS18B20_PIN);

void setup() {
    Serial.begin(115200);
    if (!sensor.begin()) {
        Serial.println("Could not find DS18B20 sensor.");
        while (1); // Halt execution if sensor is not found
    }
}

void loop() {
    float temperatureC = sensor.getTemperatureC();
    if (temperatureC != -127.0) {
        Serial.print("Temperature: ");
        Serial.print(temperatureC);
        Serial.println(" °C");
    } else {
        Serial.println("Error reading temperature");
    }
    delay(1000); // Wait a second before the next reading
}
