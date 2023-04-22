#ifndef LED_h
#define LED_h


#include "Arduino.h"
#include <FastLED.h>

class LED{
  private:
    CRGB *ring;
    int numLedsInRing;
    CRGB currentColor;

  public:
    LED();
  
    LED(CRGB r[], int nLeds);

    void setColor(CRGB color);

    void turnOff();

    CRGB getCurrentColor();
};

#endif
