#include "SSD1306.h"         // Include SSD1306 OLED display library
#include "DS18B20.h"         // Include DS18B20 temperature sensor library
#include "RotaryEncoder.h"   // Include Rotary Encoder library
#include "config.h"          // Include configuration file with pin and parameter definitions
#include "pid.h"             // Include PID control header

// Initialize display, temperature sensor, and rotary encoder
SSD1306 display;                                   // Create an instance of the OLED display
DS18B20 sensor(DS18B20_PIN);                       // Create an instance of the DS18B20 sensor
RotaryEncoder encoder(Encoder_CLK, Encoder_DT, Encoder_SW); // Initialize encoder with CLK, DT, SW pins

// Temperature settings and variables
const int minTemp = 10;          // Minimum allowable temperature setting
const int maxTemp = 25;          // Maximum allowable temperature setting
float temperature_current = 0;   // Current process temperature value
float temperature_target = 18;   // Desired target temperature
int position = 18;               // Current encoder position, initially set to target temperature
int lastPosition = encoder.getPosition(); // Previous encoder position to track changes

// Variables for filtering and PID control
byte last_CLK = 0;               // Last state of CLK pin for rotary encoder
float fn = (float)fc / fs;       // Filter frequency as ratio of cutoff to sampling frequency
float alpha = 0.0;               // EMA filter coefficient
float error_filtered_previous;   // Previous filtered error for PID calculation
float last_integral;             // Last integral value for anti-windup in PID

// Timestamps for tracking elapsed time in PID control
unsigned long time_stamp_previous = 0;
unsigned long time_stamp_current = 0;

// Function declarations
float *PID(float);               // PID control function returning pointer to control values
float calculateAlphaEMAFilter(float); // Calculates filter coefficient
void pidFunction();              // Executes the PID control actions

void setup()
{
  // Initialize rotary encoder and OLED display
  encoder.begin();
  display.begin();

  // Initialize the temperature sensor and check its presence
  if (!sensor.begin())
  {
    Serial.println("Could not find DS18B20 sensor.");
    while (1); // Stop program if sensor is not found
  }

  // Configure pins for pump and peltier control
  pinMode(pumpA, OUTPUT);
  pinMode(pumpB, OUTPUT);
  pinMode(pumpPWM, OUTPUT);
  pinMode(peltierControl, OUTPUT);

  // Prepare OLED display with permanent elements
  display.clearDisplay();
  display.drawTable();
  display.display();

  // Set up initial timestamp and calculate filter coefficient
  time_stamp_previous = millis();
  alpha = calculateAlphaEMAFilter(fn);
}

void loop()
{
  // Rotary Encoder Logic for temperature setting
  for (int i = 0; i < 9; i++)
  {
    encoder.update();            // Update encoder readings
    position += encoder.getPosition(); // Adjust position by encoder reading

    // Wrap around the position within min and max temperature limits
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

    // Check if encoder button is held down for 2 seconds to finalize setting
    if (encoder.checkButtonPressDuration() > 2000)
    {
      Serial.println("Finalise key pressed!");
      temperature_target = position;  // Update target temperature
    }

    pidFunction();                   // Run PID control function to adjust system
    delay(100);                      // Short delay between checks
  }

  // Read current temperature from DS18B20 sensor
  float tempReading = sensor.getTemperatureC();
  temperature_current = tempReading;

  if (temperature_current != -127.0) // Valid temperature reading
  {
    Serial.print("Temperature: ");
    Serial.print(temperature_current);
    Serial.println(" °C");

    display.drawText(temperature_current, 20, 28); // Display current temperature on OLED
  }
  else
  {
    Serial.println("Error reading temperature"); // Error if temperature reading fails
  }

  display.drawText(temperature_target, 80, 28); // Display target temperature on OLED
  display.display();                            // Refresh display
  delay(100);                                   // Delay for readability
}

// PID control function to compute pump and peltier control values
void pidFunction(){
  int *control_value = PID();      // Get PID control values for pump and peltier
  Serial.print("Pump Control Value: ");
  Serial.println(control_value[0]);
  Serial.print("Peltier Control Value: ");
  Serial.println(control_value[1]);

  // Set pump control signals
  digitalWrite(pumpA, HIGH);
  digitalWrite(pumpB, LOW);
  analogWrite(pumpPWM, control_value[0]);        // Set pump power

  // Set peltier control signal
  analogWrite(peltierControl, control_value[1]); // Set peltier power
}
