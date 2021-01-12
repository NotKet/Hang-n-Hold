/*
    LedLib.cpp - Library for doing something with leds
    Created by Max Maier, January 12, 2021.
    Released into the public domain.
*/

#include "Arduino.h"
#include "LedLib.h"

LedLib::LedLib(int pin) {
    pinMode(pin, OUTPUT);
    _pin = pin;
}

void LedLib::blink_once() {
    digitalWrite(_pin, HIGH);
    delay(500);
    digitalWrite(_pin, LOW);
    delay(500);
}   