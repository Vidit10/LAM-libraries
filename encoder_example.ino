#include <EncoderLibrary.h>

EncoderLibrary encoder(4, 5, 17); // Initialize with pinA, pinB, pinSwitch

void setup() {
    encoder.begin();
}

void loop() {
    encoder.update();
    
    //if (encoder.isSwitchPressed()) {
    //    Serial.println("Switch pressed");
    //}
}
