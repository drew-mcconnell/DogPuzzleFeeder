#include "Arduino.h"
#include <FastLED.h>
#include "LED.h"
#include "LightSet.h"

LightSet::LightSet()
{
  
}

//numWrongColors is RGB values to choose from for the wrong buttons
//numWrongLights is how many wrong buttons there will be (as opposed to wrong button lights just being off) 
LightSet::LightSet(int nRings, CRGB winColor, int numWinLights, CRGB *possibleWrongColors, int numWrongColors, int numWrongLights)
{
  //*_colors = new CRGB[nRings]{};

    numRings = nRings;
  
  generateSet(winColor, numWinLights, possibleWrongColors, numWrongColors, numWrongLights);
}

 LightSet::~LightSet()
 {
  //delete [] _colors;
 }

 void LightSet::generateSet(CRGB winColor, int numWinLights, CRGB *possibleWrongColors, int numWrongColors, int numWrongLights)
 {
    //-------------------TODO ----- raise exception if numWinLights is 0 or if the sum of numWinLights and numWrongLights is greater than numRings


  
  
  //Serial.println("Generating Light Set");

  //default all colors to blank
  for(int x = 0; x < numRings; x++)
  {
    _colors[x] = CRGB(0,0,0);
  }

  int numBlankLights = numRings - numWinLights - numWrongLights;

  //randomly choose win lights
  for(int i = 0; i < numWinLights; i++)
  {
    /*Serial.println("setting win lights");
    Serial.println(i+1);*/

    int randomIndex = -1;
    while(true)
    {
      randomIndex = random(0,3);

      /*Serial.print("randomIndex: ");
      Serial.println(randomIndex);*/

      //check to see if randomly chosen btn is already set, and if so, loop
      if(!colorsAreEqual(_colors[randomIndex], winColor))
      {        
        _colors[randomIndex] = winColor;

        /*Serial.print("color[");
        Serial.print(randomIndex);
        Serial.print("]: ");
        printColor(_colors[randomIndex]);*/
        
        break;
      }
    }
  }

  //randomly set wrong lights
  for(int j = 0; j < numWrongLights; j++)
  {
    /*Serial.println("setting wrong lights");
    Serial.println(j+1);*/
    
    int randomIndex;
    while(true)
    {
      randomIndex = random(0,3);

      //check to see if randomly chosen btn is already set, and if so, loop
      if(colorsAreEqual(_colors[randomIndex], CRGB(0,0,0)))
      {
        //pick from wrong colors list
        CRGB wrongColor = possibleWrongColors[random(0, numWrongColors)];
        _colors[randomIndex] = wrongColor;

        /*Serial.println("set wrong color");
        Serial.print("color[");
        Serial.print(randomIndex);
        Serial.print("]: ");
        printColor(_colors[randomIndex]);*/
        break;
      }
    }
  }
  //printSet();
 }

CRGB LightSet::getBtnColor(int index)
{
  return _colors[index];
}

void LightSet::printSet()
{
  printColor(_colors[0]);
  printColor(_colors[1]);
  printColor(_colors[2]);
}

bool LightSet::colorsAreEqual(CRGB color1, CRGB color2)
{
  return (color1.red == color2.red) && (color1.green == color2.green) && (color1.blue == color2.blue);
}

void LightSet::printColor(CRGB color1)
{
  Serial.print("Red: ");
  Serial.println(color1.red);
  Serial.print("Green: ");
  Serial.println(color1.green);
  Serial.print("Blue: ");
  Serial.println(color1.blue);
}
