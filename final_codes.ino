#include "SSD1306.h"
#include "DS18B20.h"
#include "RotaryEncoder.h"

#define DS18B20_PIN 14      // SCL - 22; SDA - 21
SSD1306 display;        // OLED Display naming
DS18B20 sensor(DS18B20_PIN);        // Temperature sensor naming
RotaryEncoder encoder(4, 5, 17); // Initialize with pinA, pinB, pinSwitch

int minTemp = 10;
int maxTemp = 25;
int currentTemp = 0;
int desiredTemp = 0;

void setup() {
    display.begin();  // Initialize the display
    encoder.begin();    // Includes the initialisation of the Serial Monitor

    if (!sensor.begin()) {
        Serial.println("Could not find DS18B20 sensor.");
        while (1); // Halt execution if sensor is not found
    }
}

void loop() {
    display.drawTable();
    display.display();
    encoder.update();
    float temperatureC = sensor.getTemperatureC();
        if (temperatureC != -127.0) {
            Serial.print("Temperature: ");
            display.drawText(temperatureC, 24, 28);       // Process value
            Serial.print(temperatureC);
            Serial.println(" °C");
        } 
        else {
            Serial.println("Error reading temperature");
        }
    display.drawText("25", 88, 28);       // Set value
    display.display();
    // delay(1000);
}