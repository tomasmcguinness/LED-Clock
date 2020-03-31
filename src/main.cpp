#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h> //I2C library
#include <RtcDS3231.h> //RTC library

#define NUM_LEDS 150
CRGB leds[NUM_LEDS];

RtcDS3231<TwoWire> rtcObject(Wire);

void setup()
{
  Serial.begin(9600);
  //FastLED.addLeds<NEOPIXEL, D2>(leds, NUM_LEDS);

  rtcObject.Begin();  

  RtcDateTime currentTime = RtcDateTime(16,05,18,21,20,0); //define date and time object
  rtcObject.SetDateTime(currentTime);  
}

void loop()
{
  RtcDateTime currentTime = rtcObject.GetDateTime();    //get the time from the RTC
 
  char str[20];   //declare a string as an array of chars
 
  sprintf(str, "%d/%d/%d %d:%d:%d",     //%d allows to print an integer to the string
          currentTime.Year(),   //get year method
          currentTime.Month(),  //get month method
          currentTime.Day(),    //get day method
          currentTime.Hour(),   //get hour method
          currentTime.Minute(), //get minute method
          currentTime.Second()  //get second method
         );
 
  Serial.println(str);

  // Serial.write("On");

  // leds[1] = CRGB::White;
  // FastLED.show();
  // delay(30);

  // Serial.write("Off");

  // leds[1] = CRGB::Black;
  // FastLED.show();
  // delay(30);

  // for (int dot = 0; dot < NUM_LEDS; dot++)
  // {
  //   leds[dot] = CRGB::Blue;
  //   FastLED.show();
  //   // clear this led for the next time around the loop
  //   leds[dot] = CRGB::Black;
  //   delay(6);
  // }

  delay(250);
}