#ifndef LightSet_h
#define LightSet_h

#include "Arduino.h"
#include <FastLED.h>
#include "LED.h"

class LightSet{
  private:

    //LED *_rings;
    CRGB _colors[3];
    int numRings;

  public:
    LightSet();
  
    LightSet(LED rings[], CRGB *ringColors, int nLeds);

    ~LightSet();
    
    //numWrongColors is RGB values to choose from for the wrong buttons
    //numWrongLights is how many wrong buttons there will be (as opposed to wrong button lights just being off) 
    LightSet(int nRings, CRGB winColor, int numWinLights, CRGB *possibleWrongColors, int numWrongColors, int numWrongLights);

    void generateSet(CRGB winColor, int numWinLights, CRGB *possibleWrongColors, int numWrongColors, int numWrongLights);

    CRGB getBtnColor(int index);

    void printSet();

    bool colorsAreEqual(CRGB color1, CRGB color2);
    void printColor(CRGB color1);
};

#endif
