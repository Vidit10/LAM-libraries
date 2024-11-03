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
    void drawText(float text, uint8_t x, uint8_t y);
    void drawTable();

private:
    void sendCommand(uint8_t command);
    void sendData(uint8_t data);
    uint8_t buffer[128 * 64 / 8]; // Buffer for the SSD1306 (128x64 pixels)
};

#endif
