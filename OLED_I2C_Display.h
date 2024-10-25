/*

#ifndef I2CDISPLAY_H
#define I2CDISPLAY_H

#include <Wire.h>
#include <Arduino.h>

class I2CDisplay
{
public:
    I2CDisplay(uint8_t address, uint8_t sdaPin = 21, uint8_t sclPin = 22);
    bool begin();
    void drawTable(const String &header1, const String &header2);
    void updateTemperature(float temp1, float temp2);
    void clearDisplay();
    void sendCommand(uint8_t command);

private:
    uint8_t _address;
    uint8_t _sdaPin;
    uint8_t _sclPin;
    void initDisplay();
    void setCursor(uint8_t x, uint8_t y);
    void sendData(uint8_t data);
    void drawLine(uint8_t y);
    void drawText(const String &text);
};

#endif

*/

#ifndef SSD1306_H
#define SSD1306_H

#include <Wire.h>

#define SSD1306_ADDRESS 0x3C // Common I2C address for SSD1306
#define SSD1306_BLACK 0
#define SSD1306_WHITE 1

class SSD1306
{
public:
    SSD1306();
    void begin();
    void clearDisplay();
    void display();
    void drawPixel(uint8_t x, uint8_t y, uint8_t color);
    void drawText(const char *text, uint8_t x, uint8_t y);

private:
    void sendCommand(uint8_t command);
    void sendData(uint8_t data);
    uint8_t buffer[128 * 64 / 8]; // Buffer for the SSD1306 (128x64 pixels)
};

#endif
