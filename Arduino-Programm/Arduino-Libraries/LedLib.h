/*
    LedLib.h - Library for doing something with leds
    Created by Max Maier, January 12, 2021.
    Released into the public domain.
*/

#ifndef LedLib_h
#define LedLib_h

#include "Arduino.h"

class LedLib {
  public:
    LedLib(int pin);
    void blink_once();
    void led_on();
  private:
    int _pin;
};

#endif