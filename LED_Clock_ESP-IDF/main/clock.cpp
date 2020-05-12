#include "FastLED.h"

#define NUM_LEDS 144
#define DATA_PIN 16
#define LED_TYPE WS2812

CRGB leds[NUM_LEDS];

int i = 0;

// void run_clock(void *pvParameters)
// {
//     while(true)
//     {
//         leds[i] =  CRGB::Red;

//         delay(1000);

//         i++;

//         if(i > NUM_LEDS)
//         {
//             FastLED.clear(true);
//             i = 0;
//         }

//         FastLED.show();
//     }
// }

// https://github.com/turiphro/alexa-led-ring-animations/blob/master/Alexa_Ring_Animations.ino
// Alexa colours
const uint32_t BOOTING_BG = CRGB(0, 0, 255);
const uint32_t BOOTING_FG = CRGB(0, 255, 255);

#define N_COLORS_CHASE 7
CRGB colors_chase[N_COLORS_CHASE] = {
    CRGB::AliceBlue,
    CRGB::Lavender,
    CRGB::DarkOrange,
    CRGB::Red,
    CRGB::Green,
    CRGB::Blue,
    CRGB::White,
};

void blinkLeds_chase(void *pvParameters)
{
    int pos = 0;
    int led_color = 0;

    while (1)
    {
        printf("chase leds\n");

        // do it the dumb way - blank the leds
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = CRGB::Black;
        }

        // set the one LED to the right color
        leds[pos] = colors_chase[led_color];
        pos = (pos + 1) % NUM_LEDS;

        // use a new color
        if (pos == 0)
        {
            led_color = (led_color + 1) % N_COLORS_CHASE;
        }

        uint64_t start = esp_timer_get_time();
        FastLED.show();
        uint64_t end = esp_timer_get_time();
        printf("Show Time: %" PRIu64 "\n", end - start);
        delay(200);
    };
}

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
    FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
    FastLED.clear(true);
    FastLED.show();

    xTaskCreatePinnedToCore(&animate_clock, "animate", 4000, NULL, 5, NULL, 0);
}

extern "C" void show_time()
{
    
}