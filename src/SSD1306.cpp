#include "SSD1306.h"

// Font data for a 5x7 font set, where each character is represented by 5 bytes.
// The data contains hex values that map to each character’s pixel layout.
const uint8_t font5x7[][5] =
{
    {0x00, 0x00, 0x00, 0x00, 0x00}, // (space)
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // "
    // ... remaining characters omitted for brevity
};

// Constructor for the SSD1306 class
SSD1306::SSD1306()
{
}

// Initializes the SSD1306 display with default settings
void SSD1306::begin()
{
    Wire.begin();                // Initialize I2C communication
    sendCommand(0xAE);           // Turn off display
    sendCommand(0xD5);           // Set display clock divide ratio
    sendCommand(0x80);           // Set default clock ratio
    sendCommand(0xA8);           // Set multiplex ratio
    sendCommand(0x3F);           // Multiplex for 128x64
    sendCommand(0xD3);           // Set display offset
    sendCommand(0x00);           // No offset
    sendCommand(0x40);           // Set display start line to 0
    sendCommand(0x8D);           // Enable charge pump regulator
    sendCommand(0x14);           // Enable charge pump
    sendCommand(0x20);           // Set memory addressing mode
    sendCommand(0x00);           // Horizontal addressing mode
    sendCommand(0xA1);           // Set segment re-map (flip horizontally)
    sendCommand(0xC8);           // Set COM output scan direction (flip vertically)
    sendCommand(0xDA);           // Set COM pins configuration
    sendCommand(0x12);           // Alternative COM pins config
    sendCommand(0x81);           // Set contrast control
    sendCommand(0xCF);           // High contrast
    sendCommand(0xD9);           // Set pre-charge period
    sendCommand(0xF1);           // Pre-charge for 0xF1
    sendCommand(0xDB);           // Set VCOMH deselect level
    sendCommand(0x40);           // 0.77 * Vcc
    sendCommand(0xA4);           // Resume to RAM content display
    sendCommand(0xA6);           // Set to normal display (not inverted)
    sendCommand(0xAF);           // Turn display on
    clearDisplay();              // Clear display buffer
    display();                   // Show the cleared display
}

// Sends a single command byte to the SSD1306 display
void SSD1306::sendCommand(uint8_t command)
{
    Wire.beginTransmission(SSD1306_ADDRESS); // Start transmission to the display
    Wire.write(0x00);                        // Co = 0, D/C# = 0
    Wire.write(command);                     // Send command byte
    Wire.endTransmission();                  // End transmission
}

// Sends a single data byte to the SSD1306 display
void SSD1306::sendData(uint8_t data)
{
    Wire.beginTransmission(SSD1306_ADDRESS); // Start transmission to the display
    Wire.write(0x40);                        // Co = 0, D/C# = 1
    Wire.write(data);                        // Send data byte
    Wire.endTransmission();                  // End transmission
}

// Clears the display buffer by setting all buffer bytes to 0
void SSD1306::clearDisplay()
{
    for (uint16_t i = 0; i < sizeof(buffer); i++)
    {
        buffer[i] = 0; // Set each byte in the buffer to 0
    }
}

// Renders the content of the buffer to the display
void SSD1306::display()
{
    sendCommand(0x21);        // Set column address
    sendCommand(0);           // Start column
    sendCommand(127);         // End column
    sendCommand(0x22);        // Set page address
    sendCommand(0);           // Start page
    sendCommand(7);           // End page

    // Write each byte in the buffer to the display
    for (uint16_t i = 0; i < sizeof(buffer); i++)
    {
        sendData(buffer[i]); // Send each byte in the buffer
    }
}

// Draws a pixel at specified x, y coordinates with a specified color
void SSD1306::drawPixel(uint8_t x, uint8_t y, uint8_t color)
{
    if (x >= 128 || y >= 64) return; // Ensure coordinates are within bounds

    switch (color)
    {
    case SSD1306_WHITE:
        buffer[x + (y / 8) * 128] |= (1 << (y & 7)); // Set pixel to white
        break;
    case SSD1306_BLACK:
        buffer[x + (y / 8) * 128] &= ~(1 << (y & 7)); // Set pixel to black
        break;
    }
}

// Draws a string of text at specified x, y coordinates
void SSD1306::drawText(const char *text, uint8_t x, uint8_t y)
{
    while (*text) // Loop through each character in the text
    {
        for (int i = 0; i < 5; i++)
        {
            // Map character to font and store in buffer
            buffer[x + (y / 8) * 128 + i] = font5x7[*text - 32][i];
        }
        x += 6; // Move to the next character position
        text++; // Advance to the next character
    }
}

// Overloaded drawText function to display floating-point numbers
void SSD1306::drawText(float text, uint8_t x, uint8_t y)
{
    char buffer[10]; // Buffer to hold converted text

    // Convert float to string with 2 decimal places
    snprintf(buffer, sizeof(buffer), "%.2f", text); // Format float to string

    // Use existing drawText function for char* strings
    drawText(buffer, x, y);
}

// Draws a simple table structure on the display
void SSD1306::drawTable()
{
    clearDisplay();                      // Clear display first

    // Draw top border of the table
    drawText("+-------------------+", 0, 0);

    // Draw table cells with vertical dividers
    for (int i = 8; i <= 8 * 2; i += 8) // Rows
    {
        drawText("|", 0, i);            // Left border
        drawText("|", 60, i);           // Center divider
        drawText("|", 120, i);          // Right border
    }
    drawText("+-------------------+", 0, 20); // Middle border

    for (int i = 8 * 3; i <= 8 * 4; i += 8) // More rows
    {
        drawText("|", 0, i);            // Left border
        drawText("|", 60, i);           // Center divider
        drawText("|", 120, i);          // Right border
    }
    drawText("+-------------------+", 0, 40); // Bottom border

    // Table headers
    drawText("PV", 24, 8);               // Process Value header
    drawText("SV", 88, 8);               // Set Value header
    drawText("Long press to select", 4, 48); // Instruction message

    // Display the table on the OLED screen
    display();
}
