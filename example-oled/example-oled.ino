#include "OLED_I2C_Display.h"

SSD1306 display;

void setup() {
    display.begin();  // Initialize the display
}

void loop() {
    display.clearDisplay();  // Clear buffer
    display.drawText("Two hours of sleep", 0, 0);  // Draw some text at the top
    display.display();  // Send buffer to display
    delay(2000);

    display.clearDisplay();
    display.drawText("Chhapaak", 0, 0);  // Draw updated text
    display.display();
    delay(2000);
}
