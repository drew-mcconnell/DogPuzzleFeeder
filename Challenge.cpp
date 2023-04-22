#include "Arduino.h"
#include <FastLED.h>
#include "LED.h"
#include "Challenge.h"

Challenge::Challenge(){

  LED led1;
  LED led2;
  LED led3;
  
  LED newLeds[] = {led1,led2,led3};

  Challenge(newLeds, 3, 1, CRGB(0,0,255));
}

//start the challenge
Challenge::Challenge(LED newLeds[], int nLeds, int challengeLevel, CRGB winColor) {

  leds = newLeds;
  btnPressCount = 0;
  _winColor = winColor;
  numLEDs = nLeds;

  //set lightSets through challengeLength
  //lightSets = lSets;
  /*for(int i = 0; i < challengeLength; i++)
  {
    lightSets[i] = lSets[i];
    
    //Serial.println("Light Set for new challenge");
    //lightSets[i].printSet();
  }*/

  challengeLength = getChallengeLength(challengeLevel);

  lightSet = generateLightSet(challengeLevel);

  //set rest of lightSets to blank
  /*for(int x = challengeLength; x < 10; x++)
  {
    lightSets[x] = ;
  }*/

  //printLightSetsOrder();

  for(int x = 0; x < 10; x++)
  {
    orderState[x] = 0;
  }
  //printOrderState();
}

int Challenge::getChallengeLength(int level)
{
  if(level == 7)
  {
    return 2;
  }
  return 1;
}

void Challenge::turnOffAllLights(){
  //Serial.println("turn off all lights");
  //leds[btnPressCount].turnOff();
  
  for(int i = 0; i < numLEDs; i++){
    leds[i].turnOff();
  }
  
  //delay(1000);
}

LightSet Challenge::generateLightSet(int challengeLevel)
{
  LightSet tempSet;
  
  if(challengeLevel == 1)
  {
    Serial.println("level 1 - all lights blue");
    //all three buttons blue
    CRGB wrongColors[] = {};
    tempSet = LightSet(numLEDs, _winColor, 3, wrongColors, 0, 0);
  }
  else if(challengeLevel == 2)
  {
    //two buttons blue, one blank
    CRGB wrongColors[] = {};
    tempSet = LightSet(numLEDs, _winColor, 2, wrongColors, 0, 0);
  }
  else if(challengeLevel == 3)
  {
    //one button blue, two blank
    CRGB wrongColors[] = {};
    tempSet = LightSet(numLEDs, _winColor, 1, wrongColors, 0, 0);
  }
  else if(challengeLevel == 4)
  {
    //two buttons blue, one red
    CRGB wrongColors[] = {CRGB(255,0,0)};
    tempSet = LightSet(numLEDs, _winColor, 2, wrongColors, 1, 1);
  }
  else if(challengeLevel == 5)
  {
    //one button blue, two red
    CRGB wrongColors[] = {CRGB(255,0,0)};
    tempSet = LightSet(numLEDs, _winColor, 1, wrongColors, 1, 2);
  }
  else if(challengeLevel == 6)
  {
    //one button blue, two random between red and green
    CRGB wrongColors[] = {CRGB(255,0,0), CRGB(0,255,0)};
    tempSet = LightSet(numLEDs, _winColor, 1, wrongColors, 2, 2);
  }

  //--------------- TODO ------ level 7 start doing a memory game. Flash a light for a split second and then he has to click it
  //                                or make him get 2 in a row correct

  return tempSet;
}

//display lights based on order and current state
void Challenge::displayChallenge(){
  //Serial.println("display challenge");

  /*for(int i = 0; i < challengeLength; i++)
  {
    Serial.print(btnOrder[i]);
  }*/
  
  turnOffAllLights();

  for(int i = 0; i < numLEDs; i++)
  { 
    /*Serial.print("i: ");
    Serial.println(i);
    Serial.print("color: ");

    printColor(lightSet.getBtnColor(i));*/
    
    leds[i].setColor(lightSet.getBtnColor(i));
  }
}

//update current state with new dog interaction (for challenges with multiple steps)
//returns true if challenge is finished
// return 1 - success
// return 2 - not complete
// return 3 - mistake
int Challenge::updateState(int btnPressed) {
  //Serial.println("update state");
  orderState[btnPressCount] = btnPressed;
  btnPressCount++;
  /*Serial.print("btnPressCount: ");
  Serial.println(btnPressCount);*/

  int successState = success();
  if(successState != 1)
  {
    displayChallenge();
  }
  
  return successState;
}

//check for completion of challenge
// return 1 - success
// return 2 - not complete
// return 3 - mistake
int Challenge::success() {
  //Serial.println("check for success");
  //printOrderState();
  //printBtnOrder();
  
  //Serial.print("btnPressCount: ");
  //Serial.println(btnPressCount);
  
  if(btnPressCount == 0)
  {
    return 2;
  }

  /*Serial.print("challengeLength: ");
  Serial.print(challengeLength);*/
  //compare the button order against the lightSet colors
  for(int x = 0; x < challengeLength; x++)
  {
    /*Serial.print("color pressed: ");
    printColor(lightSet.getBtnColor(orderState[x]-1));*/
    
    //new way of determing if challenge is successful
    if(lightSet.getBtnColor(orderState[x]-1) != _winColor)
    {
      //Serial.println("not successful");

      //if state didn't match challenge, but also didn't equal 0, we know it was a wrong button press and we should reset
      if(orderState[x] != 0)
      {
        //Serial.println("wrong move");
        resetChallenge();
        return 3;
      }
      return 2;
    }
  }
  
  //Serial.println("successful");
  turnOffAllLights();
  return 1;
}

void Challenge::resetChallenge(){
  //Serial.println("resetting challenge");
  
  for(int x = 0; x < 10; x++)
  {
    orderState[x] = 0;
  }

  btnPressCount = 0;
}

void startNextChallenge(){
  
}

void Challenge::printOrderState(){
  Serial.print("orderState: ");
  for(int x = 0; x < 10; x++){
    Serial.print(orderState[x]);
  }
  Serial.println();
}

void Challenge::printBtnOrder(){
  Serial.print("btnOrder: ");
  for(int x = 0; x < 10; x++){
    Serial.print(btnOrder[x]);
  }
  Serial.println();
}

void Challenge::printLightSetsOrder()
{
  //Serial.println("LightSetsOrder: ");
  for(int x = 0; x < challengeLength; x++)
  {
    //Serial.print(lightSets[x]);
  }
}

void Challenge::printColor(CRGB color1)
{
  Serial.print("Red: ");
  Serial.println(color1.red);
  Serial.print("Green: ");
  Serial.println(color1.green);
  Serial.print("Blue: ");
  Serial.println(color1.blue);
}
