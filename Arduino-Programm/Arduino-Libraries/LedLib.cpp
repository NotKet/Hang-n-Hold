/*
    LedLib.cpp - Library for leds
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
    if(_pin == LOW) {
        digitalWrite(_pin, HIGH);
        delay(500);
        digitalWrite(_pin, LOW);
        delay(500);
    } else {
        digitalWrite(_pin, LOW);
        delay(500);
        digitalWrite(_pin, HIGH);
        delay(500);
    } 
}

void LedLib::led_on() {
    digitalWrite(_pin, HIGH);
}