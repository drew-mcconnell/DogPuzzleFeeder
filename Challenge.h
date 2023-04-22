#ifndef Challenge_h
#define Challenge_h


#include "Arduino.h"
#include <FastLED.h>
#include "LED.h"
#include "LightSet.h"

class Challenge {
  private:

    // ----- OLD WAY TO ORDER CHALLENGE WITH INDEX OF JUST ONE LIGHT AT A TIME------
    int btnOrder[10];

    // ----- NEW WAY TO ORDER CHALLENGE WITH LIST OF LightSets ------
    CRGB _winColor;
    
    int orderState[10];
    int challengeLength;
    
    int btnPressCount;

    LED *leds;
    int numLEDs;

    LightSet lightSet;

    int getChallengeLength(int level);

  public:

    Challenge();
    
    //start the challenge
    Challenge(LED *newLeds, int nLeds, int *buttonOrder, int challengeLen);
    Challenge(LED newLeds[], int nLeds, int challengeLevel, CRGB winColor);

    LightSet generateLightSet(int challengeLevel);
 
    void turnOffAllLights();

    //display lights based on order and current state
    void displayChallenge();
    
    //update current state with new dog interaction (for challenges with multiple steps)
    //returns true if challenge is finished
    // return 1 - success
    // return 2 - not complete
    // return 3 - mistake
    int updateState(int btnPressed);

    //check for completion of challenge
    // return 1 - success
    // return 2 - not complete
    // return 3 - mistake
    int success();

    void resetChallenge();

    void startNextChallenge();

    void printOrderState();
    
    void printBtnOrder();

    void printLightSetsOrder();

    void printColor(CRGB color1);
};


#endif
