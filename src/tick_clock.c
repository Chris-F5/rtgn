#include "rtgn/tick_clock.h"

#include <time.h>

#define CLOCK CLOCK_MONOTONIC_RAW
#define NANO 1000000000

static struct timespec next;
static unsigned int nanoDelay;

void rtgn_startTickClock(unsigned int nanosecondDelay)
{
    nanoDelay = nanosecondDelay;
    clock_gettime(CLOCK, &next);
}

int rtgn_tickClock()
{
    struct timespec now;
    clock_gettime(CLOCK, &now);
    unsigned long long nowNano, nextNano;
    nowNano = now.tv_sec * NANO + now.tv_nsec;
    nextNano = next.tv_sec * NANO + next.tv_nsec;
    if(nowNano >= nextNano) {
        next.tv_nsec += nanoDelay;
        next.tv_sec += next.tv_nsec / NANO;
        next.tv_nsec %= NANO;
        return 1;
    }
    return 0;
}
