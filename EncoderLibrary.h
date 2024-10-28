#ifndef ENCODER_LIBRARY_H
#define ENCODER_LIBRARY_H

#include <Arduino.h>

class EncoderLibrary {
public:
    EncoderLibrary(int pinA, int pinB, int pinSwitch);
    void begin();
    void update();
    int getPosition() const;
    bool isClockwise() const;
    bool isSwitchPressed() const;

private:
    int _pinA;
    int _pinB;
    int _pinSwitch;
    int _encoderPosCount;
    int _pinALast;
    bool _bCW;
};

#endif // ENCODER_LIBRARY_H

