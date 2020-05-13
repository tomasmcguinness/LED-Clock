#include "FastLED.h"

#define NUM_LEDS 144
#define DATA_PIN 16
#define LED_TYPE NEOPIXEL

static const char *TAG = "clock.cpp";

CRGB leds[NUM_LEDS];

int i = 0;

// https://github.com/turiphro/alexa-led-ring-animations/blob/master/Alexa_Ring_Animations.ino
// Alexa colours
//
const uint32_t BOOTING_BG = CRGB::Blue;
const uint32_t BOOTING_FG = CRGB::DarkBlue;

void fill(uint32_t colour)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = colour;
    }
}

void spinner(uint32_t colour_bg, uint32_t colour_fg, int pos, int width)
{
    fill(colour_bg);

    for (int i = pos; i < pos + width; i++)
    {
        leds[i % NUM_LEDS] = colour_fg;
    }
}

int counter = 0;
unsigned long last_increment = millis();

const int DURATION = 1000; // animation duration (ms)
const int STEP_SIZE = DURATION / NUM_LEDS;

enum mode
{
    BOOTING,
    CLOCK,
    TIMER,
    MESSAGE
} mode;

#define SEC_PER_DAY 86400
#define SEC_PER_HOUR 3600
#define SEC_PER_MIN 60

void move_hands()
{
    // struct timeval tv;
    // struct timezone tz;
    // gettimeofday(&tv, &tz);

    // long hms = tv.tv_sec % SEC_PER_DAY;
    // hms += tz.tz_dsttime * SEC_PER_HOUR;
    // hms -= tz.tz_minuteswest * SEC_PER_MIN;
    // // mod `hms` to insure in positive range of [0...SEC_PER_DAY)
    // hms = (hms + SEC_PER_DAY) % SEC_PER_DAY;

    // // Tear apart hms into h:m:s
    // int hour = hms / SEC_PER_HOUR;
    // int min = (hms % SEC_PER_HOUR) / SEC_PER_MIN;
    // int sec = (hms % SEC_PER_HOUR) % SEC_PER_MIN; // or hms % SEC_PER_MIN

    // ESP_LOGI(TAG, "Current local time: %d:%02d:%02d\n", hour, min, sec);
}

void animate_clock(void *pvParameters)
{
    while (true)
    {
        switch (mode)
        {
        case BOOTING:
            spinner(BOOTING_BG, BOOTING_FG, counter, 12);
            break;
        case CLOCK:
            move_hands();
            break;
        default:
            break;
        }

        FastLED.show();

        if (millis() - last_increment > STEP_SIZE || millis() < last_increment)
        {
            // 0->SIZE per DURATION

            int nextCount = ++counter;

            counter = nextCount % NUM_LEDS;

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