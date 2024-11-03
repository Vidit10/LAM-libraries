#include "RotaryEncoder.h"

// Constructor for RotaryEncoder class, initializing pin numbers and default values
RotaryEncoder::RotaryEncoder(int pinA, int pinB, int pinSwitch)
    : _pinA(pinA), _pinB(pinB), _pinSwitch(pinSwitch), _encoderPosCount(0), _bCW(true),
      _pressStartTime(0), _buttonHeld(false) {}

// Initializes the rotary encoder, sets pin modes, and prepares serial communication
void RotaryEncoder::begin()
{
    pinMode(_pinA, INPUT);             // Set pin A as input for rotation detection
    pinMode(_pinB, INPUT);             // Set pin B as input for direction detection
    pinMode(_pinSwitch, INPUT);        // Set pin for switch (button) input
    _pinALast = digitalRead(_pinA);    // Initialize _pinALast with the current state of pin A
    Serial.begin(115200);              // Begin serial communication at 115200 baud rate
    Serial.println("Encoder initialized"); // Print initialization message
}

// Updates the encoder position and detects rotation direction
void RotaryEncoder::update()
{
    int aVal = digitalRead(_pinA); // Read current state of pin A

    // Check if the state of pin A has changed (indicates rotation)
    if (aVal != _pinALast)
    {
        // Determine rotation direction by comparing pin B's state to pin A's new state
        if (digitalRead(_pinB) != aVal)
        {
            _encoderPosCount++;        // Increment position for clockwise rotation
            _bCW = true;               // Set direction flag to clockwise
        }
        else
        {
            _encoderPosCount--;        // Decrement position for counterclockwise rotation
            _bCW = false;              // Set direction flag to counterclockwise
        }
        
        // Print rotation direction and position to the serial monitor
        Serial.print("Rotated: ");
        Serial.println(_bCW ? "clockwise" : "counterclockwise");
        Serial.print("Encoder Position: ");
        Serial.println(_encoderPosCount);
    }
    _pinALast = aVal; // Update last state of pin A for future comparisons

    // Check button press duration within the update loop
    // Uncomment the following line to enable button press duration check
    // checkButtonPressDuration();
}

// Returns the current position count of the encoder
int RotaryEncoder::getPosition() const
{
    return _encoderPosCount;
}

// Returns true if the last rotation was clockwise, false if counterclockwise
bool RotaryEncoder::isClockwise() const
{
    return _bCW;
}

// Checks if the switch (button) is currently pressed
bool RotaryEncoder::isSwitchPressed() const
{
    return !digitalRead(_pinSwitch); // Returns true if button is pressed (active low)
}

// Checks the duration of button press and detects short or long press
int RotaryEncoder::checkButtonPressDuration()
{
    int timeReturn = 0; // Variable to store and return the press duration

    // If the button is currently pressed
    if (isSwitchPressed())
    {
        if (!_buttonHeld)
        {   // Detect first press
            _pressStartTime = millis(); // Record the time when button was first pressed
            _buttonHeld = true;         // Set button hold flag
        }
        else if (millis() - _pressStartTime >= _longPressThreshold)
        {   // Check for long press duration
            Serial.println("Button held for more than 2 seconds");
            // Place additional actions here for long press if needed
        }
        timeReturn = millis() - _pressStartTime; // Calculate duration since button press
    }
    else
    {
        if (_buttonHeld)
        {   // Check if button was held previously
            if (millis() - _pressStartTime < _longPressThreshold)
            {   // Short press detected if duration is below threshold
                Serial.println("Button pressed briefly");
                // Place additional actions here for short press if needed
            }
            _buttonHeld = false; // Reset button hold state for the next press
        }
        timeReturn = millis() - _pressStartTime; // Update duration value after button release
    }
    return timeReturn; // Return the button press duration in milliseconds
}
