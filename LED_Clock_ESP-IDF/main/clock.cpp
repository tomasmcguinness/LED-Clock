#include "FastLED.h"
#include "sys/time.h"
#include "freertos/FreeRTOS.h"

#define NUM_LEDS 72
#define DATA_PIN 16
#define LED_TYPE NEOPIXEL

static const char *TAG = "clock.cpp";

CRGB leds[NUM_LEDS];

// https://github.com/turiphro/alexa-led-ring-animations/blob/master/Alexa_Ring_Animations.ino
// Alexa colours
//
const CHSV BOOTING_BG = CHSV(160, 255, 128);
const uint32_t BOOTING_FG = CRGB::DarkBlue;

void fill(CHSV colour)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = colour;
    }
}

uint8_t hue = 160;

void spinner(CHSV colour_bg, uint32_t colour_fg, int pos, int width)
{
    fill(colour_bg);

    // leds[pos] = CHSV(hue, 255, 255); // Draw new pixel
    // leds[pos+1] = CHSV(hue, 255, 210);
    // leds[pos+2] = CHSV(hue, 255, 180);
    // leds[pos+3] = CHSV(hue, 255, 90);
    // leds[pos+4] = CHSV(hue, 255, 50);
    // leds[pos+5] = CHSV(hue, 255, 10);
    // leds[pos+6] = CHSV(hue, 255, 0);

    for (int i = pos; i < pos + width; i++)
    {
        leds[i % NUM_LEDS] = CHSV(hue, 255, 255);;
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

static struct tm timer;

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

void move_timer()
{
    struct timeval tv;
    struct timezone tz;
    struct tm *ptm;

    gettimeofday(&tv, &tz);

    ptm = localtime(&tv.tv_sec);

    double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;

    // Initial timer works in seconds.
    //
    int remaining_seconds = difftime(mktime(&timer),mktime(ptm));

    ESP_LOGI(TAG, "Timer has %d seconds remaining", remaining_seconds);

    fill(CHSV(255, 255, 0));

    for(int i = 0; i < remaining_seconds * 2; i++)
    {
        leds[i] = BOOTING_FG;
    }

    // At less than one second left, just restore the clock.
    //
    if (remaining_seconds <= 1)
    {
        mode = CLOCK;
    }
}

void animate_clock(void *pvParameters)
{
    ESP_LOGI(TAG, "animate_clock...");

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

    //fill(BOOTING_BG);

    xTaskCreatePinnedToCore(&animate_clock, "animate", 4000, NULL, 5, NULL, 0);
}

extern "C" void set_time()
{
    ESP_LOGI(TAG, "Time has been set");

    FastLED.clear(true);
    FastLED.show();

    mode = CLOCK;
}

extern "C" void start_timer(tm tm)
{
    ESP_LOGI(TAG, "Timer requsted!");

    FastLED.clear(true);
    FastLED.show();

    timer = tm;
    mode = TIMER;
}

extern "C" void cancel_clock_timer()
{
    ESP_LOGI(TAG, "Cancel timer requsted!");

    FastLED.clear(true);
    FastLED.show();

    mode = CLOCK;
}