# LED Clock
 
## Ticking clock

#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>      //I2C library
#include <RtcDS3231.h> //RTC library

#define NUM_LEDS 120

CRGB leds[NUM_LEDS];

RtcDS3231<TwoWire> rtcObject(Wire);

void setup()
{
  Serial.begin(9600);

  FastLED.addLeds<NEOPIXEL, D4>(leds, NUM_LEDS);

  fill_solid(leds, NUM_LEDS, CRGB::Blue);

  rtcObject.Begin();

  RtcDateTime currentTime = RtcDateTime(16, 05, 18, 21, 20, 0); //define date and time object
  rtcObject.SetDateTime(currentTime);
}

int hour = 0;
int minute = 0;
int second = 0;

void loop()
{
  Serial.println("Loop()");

  FastLED.show();

  RtcDateTime currentTime = rtcObject.GetDateTime(); //get the time from the RTC

  char str[20]; //declare a string as an array of chars

  sprintf(str, "%d/%d/%d %d:%d:%d", //%d allows to print an integer to the string
          currentTime.Year(),       //get year method
          currentTime.Month(),      //get month method
          currentTime.Day(),        //get day method
          currentTime.Hour(),       //get hour method
          currentTime.Minute(),     //get minute method
          currentTime.Second()      //get second method
  );

  Serial.println(str);

  fill_solid(leds, NUM_LEDS, CRGB::Blue);

  // Fetch the current time
  //
  hour = currentTime.Hour();
  minute = currentTime.Minute();
  second = currentTime.Second() * 2;

  char secondStr[2];
  sprintf(secondStr, "%d", second);
  Serial.println(secondStr);

  // Hour
  leds[hour - 1] = CRGB::Black;
  leds[hour] = CRGB::Black;
  leds[hour + 1] = CRGB::Black;

  // Minute
  leds[minute - 1] = CRGB::Black;
  leds[minute] = CRGB::Black;
  leds[minute + 1] = CRGB::Black;

  // Seconds
  //
  leds[second] = CRGB::Black;
  leds[second + 1] = CRGB::Black;

  FastLED.show();

  delay(1000);
}