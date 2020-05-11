#include "FastLED.h"

#define NUM_LEDS 20
#define DATA_PIN 16
#define LED_TYPE WS2812B

CRGB leds[NUM_LEDS];

int i = 0;

void run_clock(void *pvParameters) 
{
    while(true)
    {
        leds[i] =  CRGB::Red;
        delay(1000);

        i++;

        if(i > NUM_LEDS) 
        {
            FastLED.clear(true);
            i = 0;
        }

        FastLED.show();
    }
}

extern "C" void setup_clock()
{
    FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
    FastLED.clear(true);
    FastLED.show();
    xTaskCreatePinnedToCore(&run_clock, "runClock", 4000, NULL, 5, NULL, 0);
}