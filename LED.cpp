#include "Arduino.h"
#include <FastLED.h>
#include "LED.h"

LED::LED(){
  
}

LED::LED(CRGB r[], int nLeds)
{
  //Serial.print("creating LED\n");
  ring = r;
  numLedsInRing = nLeds;
}

void LED::setColor(CRGB color){

  //Serial.print("turn on ring\n");
  currentColor = color;
  for (int i = 0; i < numLedsInRing; i++)
  {
    ring[i] = color;
  }
  FastLED.show();
}

void LED::turnOff(){
  
  //Serial.print("turn off ring\n");
  for (int i = 0; i < numLedsInRing; i++)
  {
    ring[i] = CRGB(0,0,0);
  }
  FastLED.show();
}

CRGB LED::getCurrentColor()
{
  return currentColor;
}
