#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include <Arduino.h> // Arduino library for pin manipulation and timing functions

// RotaryEncoder class declaration
class RotaryEncoder {
public:
    // Constructor that takes the pin numbers for the encoder channels and the switch button
    RotaryEncoder(int pinA, int pinB, int pinSwitch);
    
    // Initializes the encoder by setting up pins and default values
    void begin();
    
    // Reads the encoder pins and updates position and rotation direction
    void update();
    
    // Returns the current position of the encoder
    int getPosition() const;
    
    // Checks if the last rotation was clockwise
    bool isClockwise() const;
    
    // Returns true if the switch button is pressed
    bool isSwitchPressed() const;
    
    // Checks the duration of button press to distinguish between short and long presses
    int checkButtonPressDuration(); 

private:
    // Encoder channel pins
    int _pinA;
    int _pinB;
    
    // Pin for the encoder's switch button
    int _pinSwitch;
    
    // Tracks the current position of the encoder
    int _encoderPosCount;
    
    // Stores the last read state of pin A to detect changes in rotation
    int _pinALast;
    
    // Tracks the rotation direction (true for clockwise, false for counterclockwise)
    bool _bCW;

    // Button press tracking variables
    unsigned long _pressStartTime;          // Timestamp when button was first pressed
    bool _buttonHeld;                       // Indicates if the button is currently being held
    const unsigned long _longPressThreshold = 2000; // Threshold for long press in milliseconds (2 seconds)
};

#endif // ROTARY_ENCODER_H
