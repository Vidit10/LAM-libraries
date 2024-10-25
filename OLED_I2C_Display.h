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