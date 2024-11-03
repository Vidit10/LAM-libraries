#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include <Arduino.h>

class RotaryEncoder {
public:
    RotaryEncoder(int pinA, int pinB, int pinSwitch);
    void begin();
    void update();
    int getPosition() const;
    bool isClockwise() const;
    bool isSwitchPressed() const;
    int checkButtonPressDuration(); // New function for press duration check

private:
    int _pinA;
    int _pinB;
    int _pinSwitch;
    int _encoderPosCount;
    int _pinALast;
    bool _bCW;

    // Variables for button press duration
    unsigned long _pressStartTime;
    bool _buttonHeld;
    const unsigned long _longPressThreshold = 2000; // 2 seconds in milliseconds
};

#endif // ENCODER_LIBRARY_H
