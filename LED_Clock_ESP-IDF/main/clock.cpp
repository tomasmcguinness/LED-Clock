#include "FastLED.h"
#include "sys/time.h"
#include "freertos/FreeRTOS.h"

#define NUM_LEDS 180 //The short testing strip
#define HOUR_SIZE NUM_LEDS / 36
#define SECOND_SIZE NUM_LEDS / 60
#define MINUTE_SIZE NUM_LEDS / 60
#define DATA_PIN 16
#define LED_TYPE NEOPIXEL
#define DELAY 5

static const char *TAG = "clock.cpp";

CRGB leds[NUM_LEDS];

// https://github.com/turiphro/alexa-led-ring-animations/blob/master/Alexa_Ring_Animations.ino
// Alexa colours
//
const CHSV BOOTING_BG = CHSV(160, 255, 64);
const CHSV BOOTING_FG = CHSV(198, 81, 96);

const CHSV CLOCK_BG = CHSV(0, 0, 0);

const CHSV CLOCK_HOUR = CHSV(160, 128, 64);
const CHSV CLOCK_MINUTE = CHSV(160, 128, 64);
const CHSV CLOCK_SECOND = CHSV(160, 128, 64);

// const CHSV CLOCK_BG = CHSV(160, 128, 255);

// const CHSV CLOCK_HOUR = CHSV(0, 0, 0);
// const CHSV CLOCK_MINUTE = CHSV(0, 0, 0);
// const CHSV CLOCK_SECOND = CHSV(0, 0, 0);

void fill(CHSV colour)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = colour;
    }
}

uint8_t hue = 160;

void spinner(CHSV colour_bg, CHSV colour_fg, int pos, int width)
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
};

static mode current_mode = BOOTING;

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
    // Ignore TimeZone information for now.
    // The NTP doesn't set this.
    // hms += tz.tz_dsttime * SEC_PER_HOUR;
    // hms -= tz.tz_minuteswest * SEC_PER_MIN;
    // hms = (hms + SEC_PER_DAY) % SEC_PER_DAY;

    // Break apart hms into h:m:s
    //
    int hour = hms / SEC_PER_HOUR;

    // Change to 12 hour clock
    //
    if(hour > 12) 
    {
        hour = hour - 12;
    }

    int minute = (hms % SEC_PER_HOUR) / SEC_PER_MIN;
    int second = (hms % SEC_PER_HOUR) % SEC_PER_MIN;

    ESP_LOGD(TAG, "Time: %d:%02d:%02d", hour, minute, second);

    fill(CLOCK_BG);

    // Set the hands!
    //
    float part = (minute / 60.0) * HOUR_SIZE;

    int target_hour = round(part) + (hour * HOUR_SIZE);

    // Turn off the LEDs for the hour hand
    //
    int hour_start = target_hour - (HOUR_SIZE / 2);
    int hour_length = hour_start + HOUR_SIZE;

    for (int i = hour_start; i < hour_length; i++)
    {
        int target_led = i;
        if(i < 0) 
        {
            target_led = NUM_LEDS + i;
        }

        leds[target_led % NUM_LEDS] = CLOCK_HOUR;
    }

    // Turn off the LEDs for the minute hand
    //
    int target_minute = minute * MINUTE_SIZE;

    int minute_start = target_minute - (MINUTE_SIZE / 2);
    int minute_length = minute_start + MINUTE_SIZE;

    for (int i = minute_start; i < minute_length; i++)
    {
        int target_led = i;

        if(i < 0) 
        {
            target_led = NUM_LEDS + i;
        }

        leds[target_led % NUM_LEDS] = CLOCK_MINUTE;
    }

    // Turn off the LEDs for the second hand
    //
    int target_second = second;

    int second_start = target_second - (SECOND_SIZE / 2);
    int second_length = second_start + SECOND_SIZE;

    for (int i = second_start; i < second_length; i++)
    {
        int target_led = i;

        if(i < 0) 
        {
            target_led = NUM_LEDS + i;
        }

        leds[target_led % NUM_LEDS] = CLOCK_SECOND;
    }

    ESP_LOGD(TAG, "Hour: %d", target_hour);
    ESP_LOGD(TAG, "Minute: %d", target_minute);
    ESP_LOGD(TAG, "Second: %d", target_second);
}

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

    ESP_LOGI(TAG, "Timer has %d seconds remaining", remaining_seconds);

    fill(CHSV(255, 255, 0));

    for(int i = 0; i < remaining_seconds * 2; i++)
    {
        leds[i] = BOOTING_FG;
    }

    // At less than one second left, just restore the clock.
    // TODO Start a flashing indication that the timer is done????
    // TODO Play a sound??
    //
    if (remaining_seconds <= 1)
    {
        current_mode = CLOCK;
    }
}

void animate_clock(void *pvParameters)
{
    while (true)
    {
        switch (current_mode)
        {
            case BOOTING:
                spinner(BOOTING_BG, BOOTING_FG, counter, NUM_LEDS / 2);
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

        delay(DELAY);
    }
}

extern "C" void setup_clock()
{
    ESP_LOGI(TAG, "Clock has been told to setup!");

    current_mode = BOOTING;

    FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
    FastLED.clear(true);
    FastLED.show();

    xTaskCreatePinnedToCore(&animate_clock, "animate", 4000, NULL, 5, NULL, 0);
}

extern "C" void start_clock_ticking()
{
    ESP_LOGI(TAG, "Clock has been told to start ticking!");

    FastLED.clear(true);
    FastLED.show();

    current_mode = CLOCK;
}

extern "C" void start_timer(tm tm)
{
    ESP_LOGI(TAG, "Timer requsted!");

    FastLED.clear(true);
    FastLED.show();

    timer = tm;
    current_mode = TIMER;
}

extern "C" void cancel_clock_timer()
{
    ESP_LOGI(TAG, "Cancel timer requsted!");

    FastLED.clear(true);
    FastLED.show();

    current_mode = CLOCK;
}