#include <Arduino.h>
#include <M5Stack.h>
#include <Wire.h>

#define SERVO_ADDR 0x53
void setup() {
    M5.begin(true, false, true);


    Wire.begin(21, 22, 100000);
}

// addr 0x01 means "control the number 1 servo by us"
void Servo_write_us(uint8_t number, uint16_t us) {
    Wire.beginTransmission(SERVO_ADDR);
    Wire.write(0x00 | number);
    Wire.write(us & 0x00ff);
    Wire.write(us >> 8 & 0x00ff);
    Wire.endTransmission();
}

void loop() {
        Servo_write_us(0, 700);
     
}
