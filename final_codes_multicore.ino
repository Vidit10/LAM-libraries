#include "SSD1306.h"
#include "DS18B20.h"
#include "RotaryEncoder.h"
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>


#define DS18B20_PIN 25

// Initialize display, temperature sensor, and encoder
SSD1306 display;
DS18B20 sensor(DS18B20_PIN);
RotaryEncoder encoder(14, 27, 26); // pinA, pinB, pinSwitch

// Temperature settings
const int minTemp = 10;
const int maxTemp = 25;
volatile int currentTemp = 0;   // Process value
volatile int desiredTemp = 18;   // Set value
volatile int position = 18;     // This is equal to desiredTemp

// Task handles and mutex
TaskHandle_t TaskOLEDTemperature;
TaskHandle_t TaskRotaryEncoder;
SemaphoreHandle_t xMutex;       // Mutex for shared data

void TaskOLEDTemperatureCode(void *parameter)
{
    for (;;)
    {
        display.clearDisplay();
        display.drawTable();

        // Read temperature
        float tempReading = sensor.getTemperatureC();
        
        // Lock mutex to safely access shared variable
        xSemaphoreTake(xMutex, portMAX_DELAY);
        currentTemp = tempReading;   // Update shared `currentTemp` variable

        if (currentTemp != -127.0) {
            Serial.print("Temperature: ");
            Serial.print(currentTemp);
            Serial.println(" °C");

            // Display current temperature
            display.drawText(currentTemp, 20, 28); // Process value
        } else {
            Serial.println("Error reading temperature");
        }

        // Display desired temperature
        display.drawText(desiredTemp, 80, 28); // Set value
        display.display();

        // Release mutex
        xSemaphoreGive(xMutex);

        // Delay of 1000 ms
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void TaskRotaryEncoderCode(void *parameter)
{
    for (;;)
    {
        encoder.update();

        // Lock mutex before modifying `position` and `desiredTemp`
        xSemaphoreTake(xMutex, portMAX_DELAY);

        position += encoder.getPosition();
        
        // Wrap position within min and max limits
        if (position > maxTemp) {
            position = minTemp;
        } else if (position < minTemp) {
            position = maxTemp;
        }

        Serial.print("Encoder position: ");
        Serial.println(position);

        // If button is pressed for more than 2000 ms, update `desiredTemp`
        if (encoder.checkButtonPressDuration() > 2000) {
            Serial.println("Finalise key pressed!");
            desiredTemp = position;
        }

        // Release mutex
        xSemaphoreGive(xMutex);

        // Delay of 100 ms
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void setup()
{
    Serial.begin(115200);
    display.begin();
    encoder.begin();  // Includes the initialization of the Serial Monitor

    // Initialize temperature sensor
    if (!sensor.begin()) {
        Serial.println("Could not find DS18B20 sensor.");
        while (1);
    }

    // Create a mutex for shared variables
    xMutex = xSemaphoreCreateMutex();

    // Create task for OLED and temperature sensor on Core 0
    xTaskCreatePinnedToCore(
        TaskOLEDTemperatureCode, // Task function
        "OLED & Temp Task",      // Name of task
        10000,                   // Stack size (in bytes)
        NULL,                    // Parameter
        1,                       // Task priority
        &TaskOLEDTemperature,    // Task handle
        0);                      // Core 0

    // Create task for rotary encoder on Core 1
    xTaskCreatePinnedToCore(
        TaskRotaryEncoderCode,   // Task function
        "Encoder Task",          // Name of task
        10000,                   // Stack size (in bytes)
        NULL,                    // Parameter
        1,                       // Task priority
        &TaskRotaryEncoder,      // Task handle
        1);                      // Core 1
}

void loop()
{
    // Nothing here, tasks run independently
}
