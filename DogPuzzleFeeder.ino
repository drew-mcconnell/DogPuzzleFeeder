#include <FastLED.h>
#include <Servo.h>
#include "LED.h"
#include "Challenge.h"
#include <Wire.h>
#include <EEPROM.h>

#define NUM_LEDS 40 //2 Inch ring
#define NUM_BTNS 3 //number of buttons/LED rings

int levelAddressInMem = 0;

//using pin 11 instead of 9 or 10 because those use PWM signals if no servos are attached
//and it was creating an issue where the servo would run immediately on startup
//https://www.arduino.cc/reference/en/libraries/servo/detach/
#define PIN_SERVO 11

// The LuMini rings need two data pins connected
#define RING_DATA_PIN1 4
#define RING_DATA_PIN2 3
#define RING_DATA_PIN3 2

#define CLOCK_PIN1 5
#define CLOCK_PIN2 6
#define CLOCK_PIN3 7

unsigned long delayStart = 0;

CRGB ring1[NUM_LEDS];
CRGB ring2[NUM_LEDS];
CRGB ring3[NUM_LEDS];

LED led1;
LED led2;
LED led3;
LED leds[NUM_BTNS];

const int forceSensor1Pin = 0; // FSR is connected to analog 0
int lastForceSensor1Reading;      // the analog reading from the FSR resistor divider
const int forceSensor2Pin = 1;
int lastForceSensor2Reading;
const int forceSensor3Pin = 2;
int lastForceSensor3Reading;

Servo feedServo;

const int buzzerPin = 9; // output pin for buzzer

bool currentlyFeeding = false;
unsigned long feedStartTime = 0;
int feedRate = 150;//105; // 90+15
unsigned long feedTime = 500;//1000;//1500;//1250;   
int feedReversal = 75; // 90-15
unsigned long reversalTime = 200;//700;

Challenge c;
int currentLevel = 1;
int currentLevelInARow = 0;
int numCorrectToLevelUp = 10;
int maxLevel = 6;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Serial.print("STARTING!!!");

  //create a random seed by reading from an unused pin
  randomSeed(analogRead(5));

  feedServo.attach(PIN_SERVO);
  pinMode(buzzerPin, OUTPUT);

  //LEDS.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(ring, NUM_LEDS);
  LEDS.addLeds<APA102, RING_DATA_PIN1, CLOCK_PIN1, BGR>(ring1, NUM_LEDS);
  LEDS.addLeds<APA102, RING_DATA_PIN2, CLOCK_PIN2, BGR>(ring2, NUM_LEDS);
  LEDS.addLeds<APA102, RING_DATA_PIN3, CLOCK_PIN3, BGR>(ring3, NUM_LEDS);
  LEDS.setBrightness(20);

  //STARTUP LIGHT SEQUENCE
  for (int i = 0; i < NUM_LEDS; i++)
  {
    ring1[i] = CRGB(255,0,0);
    FastLED.show();
  }
  for (int i = 0; i < NUM_LEDS; i++)
  {
    ring1[i] = CRGB(0,0,0);
    FastLED.show();
  }
  for (int i = 0; i < NUM_LEDS; i++)
  {
    ring2[i] = CRGB(0,255,0);
    FastLED.show();
  }
  for (int i = 0; i < NUM_LEDS; i++)
  {
    ring2[i] = CRGB(0,0,0);
    FastLED.show();
  }
  for (int i = 0; i < NUM_LEDS; i++)
  {
    ring3[i] = CRGB(0,0,255);
    FastLED.show();
  }
  for (int i = 0; i < NUM_LEDS; i++)
  {
    ring3[i] = CRGB(0,0,0);
    FastLED.show();
  }

  
  led1 = LED(ring1, NUM_LEDS);
  led2 = LED(ring2, NUM_LEDS);
  led3 = LED(ring3, NUM_LEDS);

  leds[0] = led1;
  leds[1] = led2;
  leds[2] = led3;

  //STARTUP LIGHT SEQUENCE
  /*leds[0].setColor(CRGB(255,0,0));
  leds[0].turnOff();
  leds[1].setColor(CRGB(0,255,0));
  leds[1].turnOff();
  leds[2].setColor(CRGB(0,0,255));
  leds[2].turnOff();*/

   // read current level from EEPROM
   //EEPROM.write(levelAddressInMem, 1);
   byte value = EEPROM.read(levelAddressInMem);

  Serial.println(value);
  currentLevel = int(value);

  //if eeprom read is invalid, set level to 1
  if(currentLevel < 1 || currentLevel > maxLevel)
  {
    Serial.println("reseting level");
    currentLevel = 1;
    EEPROM.update(levelAddressInMem, 1);
  }
  
  Serial.print("currentLevel: ");
  Serial.println(currentLevel);
  //currentLevel = 1;
  
  c = randomChallenge(currentLevel);
  c.displayChallenge();

  //delay a bit after displaying challenge because for some reasons it raises force resistor values and causes false button presses
  delayStart = millis();
}

enum ChallengeStatus {success, incomplete, mistake};

Challenge randomChallenge(int level)
{  
  Serial.println("generating random challenge");
  Serial.print("level: ");
  Serial.println(level);
  Challenge c = Challenge(leds, NUM_BTNS, level, CRGB(0,0,255));
  
  return c;
}

int frequency(char note) 
{
  // This function takes a note character (a-g), and returns the
  // corresponding frequency in Hz for the tone() function.

  int i;
  const int numNotes = 9;  // number of notes we're storing

  // The following arrays hold the note characters and their
  // corresponding frequencies. The last "C" note is uppercase
  // to separate it from the first lowercase "c". If you want to
  // add more notes, you'll need to use unique characters.

  // For the "char" (character) type, we put single characters
  // in single quotes.

  char names[] = { 'c', 'd', 'e', 'f', 'F', 'g', 'a', 'b', 'C' };
  int frequencies[] = {262, 294, 330, 349, 370, 392, 440, 494, 523};
  /*
  note  frequency
  c     262 Hz
  d     294 Hz
  e     330 Hz
  f     349 Hz
  g     392 Hz
  a     440 Hz
  b     494 Hz
  C     523 Hz
  */

  // Now we'll search through the letters in the array, and if
  // we find it, we'll return the frequency for that note.

  for (i = 0; i < numNotes; i++)  // Step through the notes
  {
    if (names[i] == note)         // Is this the one?
    {
      return(frequencies[i]);     // Yes! Return the frequency
    }
  }
  return(0);  // We looked through everything and didn't find it,
              // but we still need to return a value, so return 0.
}

unsigned long timeElapsedSince(unsigned long startDelayTime)
{
  //Serial.println("getting elapsed time");
  return millis() - startDelayTime;
}

void loop() {
    
    int forceSensor1Reading;
    int forceSensor2Reading;
    int forceSensor3Reading;
  
  //-------------TODO - FIGURE OUT A FEED SYSTEM AT STARTUP TO GET FOOD TO FRONT OF AUGER ----------
  if(timeElapsedSince(delayStart) < 150)
  {
    //feedServo.write(feedRate);
    lastForceSensor1Reading = analogRead(forceSensor1Pin);
    lastForceSensor2Reading = analogRead(forceSensor2Pin);
    lastForceSensor3Reading = analogRead(forceSensor3Pin);
  }

  
  //Only check sensors and do stuff if we're not delaying
  else if(timeElapsedSince(delayStart) > 150 && !currentlyFeeding)
  {
    forceSensor1Reading = analogRead(forceSensor1Pin);
    forceSensor2Reading = analogRead(forceSensor2Pin);
    forceSensor3Reading = analogRead(forceSensor3Pin);
  
    Serial.print("sensor 1: ");
    Serial.print(lastForceSensor1Reading);
    Serial.print(" ");
    Serial.println(forceSensor1Reading);
    Serial.print("sensor 2: ");
    Serial.print(lastForceSensor2Reading);
    Serial.print(" ");
    Serial.println(forceSensor2Reading);
    Serial.print("sensor 3: ");
    Serial.print(lastForceSensor3Reading);
    Serial.print(" ");
    Serial.println(forceSensor3Reading);
    Serial.print("\n");
    //delay(300);
    
    int success = 0;
    bool mistake = false;

    //discern that a button has been press if the most recent force reading is thresholdDifference greater than the previous reading
    const int thresholdDifference = 19;
    
    if (forceSensor1Reading >= lastForceSensor1Reading + thresholdDifference)
    {
      Serial.println("button 1 pressed");
       success = c.updateState(1);
       
       //delay(100);
       delayStart = millis();

       /*Serial.print("success = ");
       Serial.println(success);*/
    }
    else if (forceSensor2Reading >= lastForceSensor2Reading + thresholdDifference)
    {
      Serial.println("button 2 pressed");
       success = c.updateState(2);
       
       //delay(100);
       delayStart = millis();

       /*Serial.print("success = ");
       Serial.println(success);*/
    }
    else if (forceSensor3Reading >= lastForceSensor3Reading + thresholdDifference)
    {
      Serial.println("button 3 pressed");
       success = c.updateState(3);
       
       //delay(100);
       delayStart = millis();

       /*Serial.print("success = ");
       Serial.println(success);*/
    }
  
    if(success == 1)
    {
       Serial.println("SUCCESS!!");
       //Serial.print(frequency('C'));
       tone(buzzerPin, frequency('C'), 500); 
       //tone(buzzerPin, frequency('g'), 100);

       currentLevelInARow++;
       if(currentLevelInARow >= numCorrectToLevelUp)
       {
        if(currentLevel < maxLevel)
        {
          currentLevel++;
          currentLevelInARow = 0;
        }

        // write new level to EEPROM
        EEPROM.update(levelAddressInMem, currentLevel);
       }
       c = randomChallenge(currentLevel);
       //c.resetChallenge();

       //Serial.print("currentLevel = ");
       //Serial.println(currentLevel);
       //Serial.print("currentLevelInARow = ");
       //Serial.println(currentLevelInARow);
       //challenges[currentChallenge].resetChallenge();
       //currentChallenge++;

       /*if(currentChallenge == totalChallenges)
       {
        //TODO - if completed the last challenge, randomize challenge or go back to beginning
        //currently just resetting to the first challenge 
          currentChallenge = 0;
          Serial.print("going back to first challenge");
       }*/
       
       //START FEEDING
       //feed();
       Serial.println("feeding");
       currentlyFeeding = true;
       feedStartTime = millis();
       feedServo.write(feedRate);
    }
    else if(success == 3)
    {
      Serial.println("MADE A MISTAKE");

      //play "incorrect" sound
      tone(buzzerPin, frequency('F'), 100);
       
      currentLevelInARow = 0;

      /*Serial.print("currentLevel = ");
      Serial.println(currentLevel);
      Serial.print("currentLevelInARow = ");
      Serial.println(currentLevelInARow);*/
    }
  
    //delay(500);
    //printData();
  }
  else if(currentlyFeeding)
  {
    //Serial.println("currently feeding");
    unsigned long feedingTimeElapsed = timeElapsedSince(feedStartTime);
    //Serial.print("feedingTimeElapsed: ");
    //Serial.println(feedingTimeElapsed);
    
    //rotate forward for feedTime, then reverse for reversalTime
    if(feedingTimeElapsed > feedTime && feedingTimeElapsed < feedTime + reversalTime)
    {
        //Serial.println("reverse");
        feedServo.write(feedReversal);
    }
    //then rotate forward again to counteract reversal
    else if(feedingTimeElapsed >= feedTime + reversalTime && feedingTimeElapsed < feedTime + reversalTime + reversalTime)
    {
        //Serial.println("unreverse");
        //feedServo.write(feedRate);

        //TRYING - STOP MOTOR AFTER REVERSE INSTEAD OF MOVING IT BACK FORWARD TO SEE IF I CAN PREVENT MORE FOOD COMING OUT
        feedServo.write(90);
    }
    //then stop feeding
    else if(feedingTimeElapsed >= feedTime + reversalTime + reversalTime + 2000)
    {
        //Serial.println("stop feeding");
        feedServo.write(90);
        currentlyFeeding = false;
        
        c.displayChallenge();

        //delay a bit after displaying challenge because for some reasons it raises force resistor values and causes false button presses
        delayStart = millis();
    }
  }

    lastForceSensor1Reading = forceSensor1Reading;
    lastForceSensor2Reading = forceSensor2Reading;
    lastForceSensor3Reading = forceSensor3Reading;
}
