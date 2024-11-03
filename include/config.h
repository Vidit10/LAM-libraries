/*
    Define all variables and constants here, including pin configurations.
    Naming Convention:
        - Pin Configuration
        - Starting letter in uppercase with multiple words separated by underscores.
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h> // Includes core Arduino functions
#include <Wire.h>    // Required for I2C communication
#include <OneWire.h> // Required for OneWire communication protocol

// === Pin Definitions ===
// Rotary Encoder pins
#define Encoder_CLK 14    // Clock pin for the rotary encoder
#define Encoder_DT 27     // Data pin for the rotary encoder
#define Encoder_SW 26     // Switch/button pin for the rotary encoder

// Pump control pins
#define pumpA 2           // Pump control pin A
#define pumpB 4           // Pump control pin B
#define pumpPWM 19        // PWM pin for controlling pump speed

// Peltier control pin
#define peltierControl 15 // PWM pin for Peltier module control

// === Sampling Frequency ===
#define fs 6000           // Default sampling frequency for the ESP32 (in Hz)
#define fc 50             // Cutoff frequency for the filter (in Hz)

// DS18B20 Temperature Sensor Pin
#define DS18B20_PIN 25    // Pin for the DS18B20 temperature sensor

// === PID Controller Constants for Pump ===
#define pump_Kp 200       // Proportional gain for pump
#define pump_Ki 0.12         // Integral gain for pump
#define pump_Kd 0.03         // Derivative gain for pump
#define pump_max_output 255 // Maximum output value for pump PWM (0-255)
#define pump_min_output 0   // Minimum output value for pump PWM (0-255)

// === PID Controller Constants for Peltier ===
#define peltier_Kp 200       // Proportional gain for Peltier
#define peltier_Ki 0.3         // Integral gain for Peltier
#define peltier_Kd 0.02         // Derivative gain for Peltier
#define peltier_max_output 255 // Maximum output for Peltier PWM (0-255)
#define peltier_min_output 0   // Minimum output for Peltier PWM (0-255)

#endif
