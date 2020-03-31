#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 60
CRGB leds[NUM_LEDS];

void setup() 
{ 
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, D2>(leds, NUM_LEDS); 
}

void loop() 
{
  Serial.write("On");

	leds[1] = CRGB::White; 
  FastLED.show(); 
  delay(30);
	
  Serial.write("Off");

  leds[1] = CRGB::Black; 
  FastLED.show(); 
  delay(30);
}