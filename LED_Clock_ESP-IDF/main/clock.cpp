#include "FastLED.h"

#define NUM_LEDS 144
#define DATA_PIN 16
#define LED_TYPE WS2812

static const char *TAG = "clock.cpp";

CRGB leds[NUM_LEDS];

int i = 0;

// https://github.com/turiphro/alexa-led-ring-animations/blob/master/Alexa_Ring_Animations.ino
// Alexa colours
const uint32_t BOOTING_BG = CRGB(0, 0, 255);
const uint32_t BOOTING_FG = CRGB(0, 255, 255);

void fill(uint32_t colour)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = colour;
    }
}

void spinner(uint32_t colour_bg, uint32_t colour_fg, int pos, int width = ceil(NUM_LEDS / 12.0));
void spinner(uint32_t colour_bg, uint32_t colour_fg, int pos, int width)
{
    fill(colour_bg);

    for (int i = pos; i < pos + width; i++)
    {
        leds[i % NUM_LEDS] = colour_fg; // rotate clockwise
    }
}

int counter = 0;
unsigned long last_increment = millis();

const int DURATION = 1000; // animation duration (ms)
const int STEP_SIZE = DURATION / NUM_LEDS;

enum mode {
  BOOTING,
  CLOCK,
  TIMER,
  MESSAGE
} mode;

void animate_clock(void *pvParameters)
{
    while (true)
    {
        spinner(BOOTING_BG, BOOTING_FG, counter);
        FastLED.show();

        if (millis() - last_increment > STEP_SIZE || millis() < last_increment)
        {
            // 0->SIZE per DURATION

            int nextCount = ++counter;

            counter = nextCount % NUM_LEDS;

            //counter = (++counter % NUM_LEDS);
            if (counter == 0)
            {
                //up = !up; // swap
            }

            last_increment = millis();
        }

        delay(1);
    }
}

extern "C" void setup_clock()
{
    mode = BOOTING;

    FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
    FastLED.clear(true);
    FastLED.show();

    xTaskCreatePinnedToCore(&animate_clock, "animate", 4000, NULL, 5, NULL, 0);
}

extern "C" void set_time()
{
    ESP_LOGI(TAG, "Time has been set");

    mode = CLOCK;
}