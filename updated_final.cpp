#include "SSD1306.h"
#include "DS18B20.h"
#include "RotaryEncoder.h"
#include "config.h"
#include "pid.h"
#include "Teleplot.h"
#include <math.h>

// Temperature settings
const int minTemp = 10;
const int maxTemp = 25;
float currentTemp = 0;  // Process value
float desiredTemp = 18; // Set value
float position = 18;    // This is equal to desiredTemp

float fn = (float)fc / fs;
float alpha = 0.0;
float error_filtered_previous;
float last_integral;

unsigned long time_stamp_previous = 0;
unsigned long time_stamp_current = 0;

float *PID(float);
float calculateAlphaEMAFilter(float);

Teleplot teleplot("127.0.0.1");

void setup()
{
  encoder.begin(); // Sets up the serial monitor to a baud rate of 115200
  display.begin();

  // Initialize temperature sensor
  if (!sensor.begin())
  {
    Serial.println("Could not find DS18B20 sensor.");
    while (1)
      ;
  }

  pinMode(pumpA, OUTPUT);
  pinMode(pumpB, OUTPUT);
  pinMode(pumpPWM, OUTPUT);

  pinMode(peltierControl, OUTPUT);

  display.clearDisplay();
  display.drawTable(); // The permanent components of the screen do not get refreshed everytime
  display.display();

  time_stamp_previous = millis();
  alpha = calculateAlphaEMAFilter(fn);
}

void loop()
{
  // display.clearDisplay();
  // display.drawTable();

  for (int i = 0; i < 9; i++)
  {
    // === Rotary Encoder Logic ===
    encoder.update();
    position += encoder.getPosition();

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

    if (encoder.checkButtonPressDuration() > 2000)
    {
      Serial.println("Finalise key pressed!");
      desiredTemp = position;
    }
    delay(100);
  }

  // Read temperature
  float tempReading = sensor.getTemperatureC();
  currentTemp = tempReading;

  if (currentTemp != -127.0)
  {
    Serial.print("Temperature: ");
    Serial.print(currentTemp);
    Serial.println(" °C");

    display.drawText(currentTemp, 20, 28); // Process value
  }
  else
  {
    Serial.println("Error reading temperature");
  }

  // Display desired temperature
  display.drawText(desiredTemp, 80, 28); // Set value
  display.display();
  delay(100);
}
