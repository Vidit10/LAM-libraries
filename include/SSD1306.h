#ifndef SSD1306_H
#define SSD1306_H

#include <Wire.h>               // Include Wire library for I2C communication

// Define the I2C address for the SSD1306 display
#define SSD1306_ADDRESS 0x3C    // Common I2C address for SSD1306 OLED displays

// Define color values for pixels (0 = black, 1 = white)
#define SSD1306_BLACK 0
#define SSD1306_WHITE 1

// SSD1306 class declaration
class SSD1306
{
public:
    SSD1306();                     // Constructor to initialize the SSD1306 instance
    void begin();                  // Initializes the display with required settings
    void clearDisplay();           // Clears the display buffer (fills with black)
    void display();                // Pushes the buffer data to the display for rendering
    void drawPixel(uint8_t x, uint8_t y, uint8_t color); // Draws a pixel at specified coordinates with given color
    void drawText(const char *text, uint8_t x, uint8_t y); // Draws a text string at specified coordinates
    void drawText(float text, uint8_t x, uint8_t y); // Overloaded method to draw floating-point numbers as text
    void drawTable();              // Draws static UI components (e.g., table structure) on the display

private:
    void sendCommand(uint8_t command); // Sends a command byte to the display via I2C
    void sendData(uint8_t data);       // Sends data byte to the display via I2C
    uint8_t buffer[128 * 64 / 8];      // Buffer to store pixel data (128x64 pixels, 1-bit per pixel)
};

#endif
