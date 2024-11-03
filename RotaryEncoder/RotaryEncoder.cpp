#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(int pinA, int pinB, int pinSwitch)
    : _pinA(pinA), _pinB(pinB), _pinSwitch(pinSwitch), _encoderPosCount(0), _bCW(true),
      _pressStartTime(0), _buttonHeld(false) {}

void RotaryEncoder::begin()
{
    pinMode(_pinA, INPUT);
    pinMode(_pinB, INPUT);
    pinMode(_pinSwitch, INPUT);
    _pinALast = digitalRead(_pinA);
    Serial.begin(115200);
    Serial.println("Encoder initialized");
}

void RotaryEncoder::update()
{
    int aVal = digitalRead(_pinA);
    if (aVal != _pinALast)
    { // If the knob is rotating
        // Determine direction by reading pin B
        if (digitalRead(_pinB) != aVal)
        { // Clockwise rotation
            _encoderPosCount++;
            _bCW = true;
        }
        else
        { // Counterclockwise rotation
            _encoderPosCount--;
            _bCW = false;
        }
        // Display rotation direction and position
        Serial.print("Rotated: ");
        Serial.println(_bCW ? "clockwise" : "counterclockwise");
        Serial.print("Encoder Position: ");
        Serial.println(_encoderPosCount);
    }
    _pinALast = aVal;

    // Call button press duration check within update
    checkButtonPressDuration();
}

int RotaryEncoder::getPosition() const
{
    return _encoderPosCount;
}

bool RotaryEncoder::isClockwise() const
{
    return _bCW;
}

bool RotaryEncoder::isSwitchPressed() const
{
    return !digitalRead(_pinSwitch);
}

// New function to check button press duration
int RotaryEncoder::checkButtonPressDuration()
{
    int timeReturn = 0;
    if (isSwitchPressed())
    { // Button is pressed
        if (!_buttonHeld)
        {                               // First press detection
            _pressStartTime = millis(); // Record the time of press
            _buttonHeld = true;
        }
        else if (millis() - _pressStartTime >= _longPressThreshold)
        { // Long press detection
            Serial.println("Button held for more than 2 seconds");
            // Additional actions for long press
        }
        timeReturn = millis() - _pressStartTime;
    }
    else
    { // Button is not pressed
        if (_buttonHeld)
        { // Previously held
            if (millis() - _pressStartTime < _longPressThreshold)
            { // Short press detection
                Serial.println("Button pressed briefly");
                // Additional actions for short press
            }
            _buttonHeld = false; // Reset the hold state
        }
        timeReturn = millis() - _pressStartTime;
    }
    return timeReturn;
}
