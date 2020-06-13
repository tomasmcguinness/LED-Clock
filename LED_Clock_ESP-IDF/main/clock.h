#include "sys/time.h"

void setup_clock();
void start_clock_ticking();
void start_timer(struct tm tm);
void cancel_clock_timer();