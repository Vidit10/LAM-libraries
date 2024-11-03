#include "SSD1306.h"
#include "DS18B20.h"
#include "RotaryEncoder.h"
// #include <config.h>
// #include <math.h>
// #include <pid.h>
// #include <Teleplot.h>

#define DS18B20_PIN 25

// Initialize display, temperature sensor, and encoder
SSD1306 display;
DS18B20 sensor(DS18B20_PIN);
RotaryEncoder encoder(14, 27, 26); // pinA, pinB, pinSwitch

// Temperature settings
const int minTemp = 10;
const int maxTemp = 25;
volatile int currentTemp = 0;  // Process value
volatile int desiredTemp = 18; // Set value
volatile int position = 18;    // This is equal to desiredTemp

// byte last_CLK = 0;

// float fn = (float)fc / fs;
// float alpha = 0.0;
// float error_filtered_previous;
// float last_integral;

// unsigned long time_stamp_previous = 0;
// unsigned long time_stamp_current = 0;

// float* PID(float);
// float calculateAlphaEMAFilter(float);

// Teleplot teleplot("127.0.0.1");

void setup()
{
    Serial.begin(115200);
    display.begin();
    encoder.begin();
    pinMode(15, OUTPUT);
    pinMode(2, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(19, OUTPUT);

    // Initialize temperature sensor
    if (!sensor.begin())
    {
        Serial.println("Could not find DS18B20 sensor.");
        while (1)
            ;
    }
    // pinMode(pumpA, OUTPUT);
    // pinMode(pumpB, OUTPUT);
    // pinMode(pumpPWM, OUTPUT);

    // pinMode(peltierControl, OUTPUT);
    // display.clearDisplay();
    // display.drawTable();

    // time_stamp_previous = millis();
    // alpha = calculateAlphaEMAFilter(fn);
}

void loop()
{
    // === Temperature Display and Reading ===
    display.clearDisplay();
    display.drawTable();

    // int *control_value;
    // control_value = PID();
    // Serial.print("Pump Control Value");
    // Serial.println(control_value[0]);
    // Serial.print("Peltier Control Value");
    // Serial.println(control_value[1]);

    // // Read temperature
    float tempReading = sensor.getTemperatureC();
    currentTemp = tempReading; // Update shared `currentTemp` variable

    if (currentTemp != -127.0)
    {
        Serial.print("Temperature: ");
        Serial.print(currentTemp);
        Serial.println(" °C");

        // Display current temperature
        display.drawText(currentTemp, 20, 28); // Process value
    }
    else
    {
        Serial.println("Error reading temperature");
    }

    // Display desired temperature
    display.drawText(desiredTemp, 80, 28); // Set value
    display.display();

    // === Rotary Encoder Logic ===
    encoder.update();
    position += encoder.getPosition();

    // Wrap position within min and max limits
    if (position > maxTemp)
    {
        position = minTemp;
    }
    else if (position < minTemp)
    {
        position = maxTemp;
    }

    Serial.print("Encoder position: ");
    Serial.println(position);

    // If button is pressed for more than 2000 ms, update `desiredTemp`
    if (encoder.checkButtonPressDuration() > 2000)
    {
        Serial.println("Finalise key pressed!");
        desiredTemp = position;
    }

    // digitalWrite(pumpA, HIGH);
    // digitalWrite(pumpB, LOW);
    // analogWrite(pumpPWM, 200);
    // analogWrite(peltierControl, 100);
    // delay(5000);
    // analogWrite(pumpPWM, 100);
    // analogWrite(peltierControl, 100);
    // delay(5000);
    // teleplot.update("Current Temperature", temperature_current);

    // Peltier
    digitalWrite(15, HIGH);
    // Motor driver
    digitalWrite(4, LOW);
    digitalWrite(2, HIGH);
    analogWrite(19, 190);

    // Delay for 100 ms to give some time for each loop iteration
    delay(100);
}
