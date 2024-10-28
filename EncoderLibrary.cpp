#include "EncoderLibrary.h"

EncoderLibrary::EncoderLibrary(int pinA, int pinB, int pinSwitch)
    : _pinA(pinA), _pinB(pinB), _pinSwitch(pinSwitch), _encoderPosCount(0), _bCW(true) {}

void EncoderLibrary::begin() {
    pinMode(_pinA, INPUT);
    pinMode(_pinB, INPUT);
    pinMode(_pinSwitch, INPUT);
    _pinALast = digitalRead(_pinA);
    Serial.begin(115200);
    Serial.println("Encoder initialized");
}

void EncoderLibrary::update() {
    int aVal = digitalRead(_pinA);
    if (aVal != _pinALast) { // If the knob is rotating
        // Determine direction by reading pin B
        if (digitalRead(_pinB) != aVal) { // Clockwise rotation
            _encoderPosCount++;
            _bCW = true;
        } else { // Counterclockwise rotation
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
}

int EncoderLibrary::getPosition() const {
    return _encoderPosCount;
}

bool EncoderLibrary::isClockwise() const {
    return _bCW;
}

bool EncoderLibrary::isSwitchPressed() const {
    return !digitalRead(_pinSwitch);
}

