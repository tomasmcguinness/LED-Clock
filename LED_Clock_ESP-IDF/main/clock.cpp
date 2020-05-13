#include "FastLED.h"
#include "sys/time.h"
#include "freertos/FreeRTOS.h"

#define NUM_LEDS 120
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
    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);

    long hms = tv.tv_sec % SEC_PER_DAY;
    hms += tz.tz_dsttime * SEC_PER_HOUR;
    hms -= tz.tz_minuteswest * SEC_PER_MIN;
    // mod `hms` to insure in positive range of [0...SEC_PER_DAY)
    hms = (hms + SEC_PER_DAY) % SEC_PER_DAY;

    // Tear apart hms into h:m:s
    int hour = hms / SEC_PER_HOUR;
    int minute = (hms % SEC_PER_HOUR) / SEC_PER_MIN;
    int second = (hms % SEC_PER_HOUR) % SEC_PER_MIN; // or hms % SEC_PER_MIN

    //ESP_LOGI(TAG, "Current local time: %d:%02d:%02d\n", hour, min, sec);

    fill(BOOTING_BG);

    // Set the hands!
    //
    float part = (minute / 60.0) * 12;

    int target_hour = round(part) + (hour * 12);

    for (int i = target_hour - 1; i < target_hour + 2; i++)
    {
        leds[i % NUM_LEDS] = CRGB::Black;
    }

    int target_second = second * 2;

    for (int i = target_second - 1; i < target_second + 2; i++)
    {
        leds[i % NUM_LEDS] = CRGB::Black;
    }

    int target_minute = minute * 2;

    for (int i = target_minute - 1; i < target_minute + 2; i++)
    {
        leds[i % NUM_LEDS] = CRGB::Black;
    }
}

static struct tm timer;

void move_timer()
{
    struct timeval tv;
    struct timezone tz;
    struct tm *ptm;

    gettimeofday(&tv, &tz);

    ptm = localtime(&tv.tv_sec);

    // Initial timer works in seconds.
    //
    int remaining_seconds = difftime(mktime(&timer),mktime(ptm));

    //ESP_LOGI(TAG, "Timer has %d seconds remaining", remaining_seconds);

    fill(CRGB::Black);

    for(int i = 0; i < remaining_seconds * 2; i++)
    {
        leds[i] = BOOTING_FG;
    }

    if (remaining_seconds <= 0)
    {
        mode = CLOCK;
    }
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
        case TIMER:
            move_timer();
            break;
        default:
            break;
        }

        FastLED.show();

        if (millis() - last_increment > STEP_SIZE || millis() < last_increment)
        {
            int nextCount = ++counter;

            counter = nextCount % NUM_LEDS;

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

    FastLED.clear(true);
    FastLED.show();

    mode = CLOCK;
}

extern "C" void start_timer(char *timer_token, tm tm)
{
    ESP_LOGI(TAG, "Timer requsted!");

    FastLED.clear(true);
    FastLED.show();

    timer = tm;
    mode = TIMER;
}