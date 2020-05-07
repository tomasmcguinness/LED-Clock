#include <Arduino.h>
#include <FastLED.h>
#include <Wire.h>      //I2C library
#include <RtcDS3231.h> //RTC library
#include <math.h>

#define NUM_LEDS 120

CRGB leds[NUM_LEDS];

RtcDS3231<TwoWire> rtcObject(Wire);

int hour = 0;
int minute = 0;
int second = 0;

void setup()
{
  Serial.begin(9600);

  FastLED.addLeds<NEOPIXEL, D4>(leds, NUM_LEDS);
  FastLED.setBrightness(128);

  fill_solid(leds, NUM_LEDS, CRGB::Blue);

  rtcObject.Begin();

  RtcDateTime currentTime = RtcDateTime(20, 04, 26, 07, 42, 0); //define date and time object
  rtcObject.SetDateTime(currentTime);
}

void loop()
{
  // RtcDateTime currentTime = rtcObject.GetDateTime(); //get the time from the RTC

  // char str[20]; //declare a string as an array of chars

  // sprintf(str, "%d/%d/%d %d:%d:%d", //%d allows to print an integer to the string
  //         currentTime.Year(),       //get year method
  //         currentTime.Month(),      //get month method
  //         currentTime.Day(),        //get day method
  //         currentTime.Hour(),       //get hour method
  //         currentTime.Minute(),     //get minute method
  //         currentTime.Second()      //get second method
  // );

  //Serial.println(str);

  uint32_t on_color = CRGB::Red;
  uint32_t off_color = CRGB::Black;

  fill_solid(leds, NUM_LEDS, off_color);

  // Fetch the current time
  //
  // hour = currentTime.Hour();
  // minute = currentTime.Minute() * 2;
  // second = currentTime.Second() * 2;

int second_led = second * 2;
int minute_led = minute * 2;

  // Hour
  // leds[hour - 1] = on_color;
  // leds[hour] = on_color;
  // leds[hour + 1] = on_color; 
  // leds[hour + 2] = on_color;

  float part = (minute / 60.0) * 12;

  Serial.print(part, 3);

  int target_hour = round(part) + (hour * 12);

  // Hour
  leds[target_hour - 1] = on_color;
  leds[target_hour] = on_color;
  leds[target_hour + 1] = on_color; 

  // Minute
  leds[minute_led] = on_color;
  leds[minute_led + 1] = on_color;

  // Seconds
  //
  leds[second_led] = on_color;
  leds[second_led + 1] = on_color;

  FastLED.show();

  delay(1);

  second++;

  if(second == 60) {
    minute++;

    if(minute == 60) {
      
      minute = 0;

      hour++;

      if(hour == 12) {
        hour = 0;
      }
    }

    second = 0;
  }
}