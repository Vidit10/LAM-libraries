// I2CDisplay.cpp

#include "OLED_I2C_Display.h"

// SSD1306 basic commands
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_SEGREMAP 0xA1
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_NORMALDISPLAY 0xA6

I2CDisplay::I2CDisplay(uint8_t address, uint8_t sdaPin, uint8_t sclPin)
    : _address(address), _sdaPin(sdaPin), _sclPin(sclPin) {}

bool I2CDisplay::begin()
{
    Wire.begin(_sdaPin, _sclPin);
    initDisplay();
    return true;
}

void I2CDisplay::initDisplay()
{
    sendCommand(SSD1306_DISPLAYOFF);
    sendCommand(SSD1306_SETDISPLAYCLOCKDIV);
    sendCommand(0x80);
    sendCommand(SSD1306_SETMULTIPLEX);
    sendCommand(0x3F);
    sendCommand(SSD1306_SETDISPLAYOFFSET);
    sendCommand(0x00);
    sendCommand(SSD1306_SETSTARTLINE | 0x00);
    sendCommand(SSD1306_CHARGEPUMP);
    sendCommand(0x14);
    sendCommand(SSD1306_MEMORYMODE);
    sendCommand(0x00);
    sendCommand(SSD1306_SEGREMAP | 0x1);
    sendCommand(SSD1306_COMSCANDEC);
    sendCommand(SSD1306_SETCOMPINS);
    sendCommand(0x12);
    sendCommand(SSD1306_SETCONTRAST);
    sendCommand(0xCF);
    sendCommand(SSD1306_SETPRECHARGE);
    sendCommand(0xF1);
    sendCommand(SSD1306_SETVCOMDETECT);
    sendCommand(0x40);
    sendCommand(SSD1306_DISPLAYALLON_RESUME);
    sendCommand(SSD1306_NORMALDISPLAY);
    sendCommand(SSD1306_DISPLAYON);
}

void I2CDisplay::clearDisplay()
{
    for (uint16_t i = 0; i < 1024; i++)
    {
        sendData(0x00);
    }
}

void I2CDisplay::drawTable(const String &header1, const String &header2)
{
    clearDisplay();
    drawLine(0);
    setCursor(0, 1);
    drawText(header1);
    setCursor(64, 1);
    drawText(header2);
    drawLine(2);
}

void I2CDisplay::updateTemperature(float temp1, float temp2)
{
    char tempStr1[8], tempStr2[8];
    snprintf(tempStr1, sizeof(tempStr1), "%.2f", temp1);
    snprintf(tempStr2, sizeof(tempStr2), "%.2f", temp2);
    setCursor(0, 3);
    drawText(tempStr1);
    setCursor(64, 3);
    drawText(tempStr2);
}

void I2CDisplay::setCursor(uint8_t x, uint8_t y)
{
    sendCommand(0xB0 + y);
    sendCommand(0x00 + (x & 0x0F));
    sendCommand(0x10 + ((x >> 4) & 0x0F));
}

void I2CDisplay::sendCommand(uint8_t command)
{
    Wire.beginTransmission(_address);
    Wire.write(0x00); // Co = 0, D/C = 0
    Wire.write(command);
    Wire.endTransmission();
}

void I2CDisplay::sendData(uint8_t data)
{
    Wire.beginTransmission(_address);
    Wire.write(0x40); // Co = 0, D/C = 1
    Wire.write(data);
    Wire.endTransmission();
}

void I2CDisplay::drawText(const String &text)
{
    for (uint8_t i = 0; i < text.length(); i++)
    {
        sendData(text[i]);
    }
}

void I2CDisplay::drawLine(uint8_t y)
{
    setCursor(0, y);
    for (uint8_t i = 0; i < 128; i++)
    {
        sendData(0xFF);
    }
}
