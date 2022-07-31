#include <stdio.h>

#include <rtgn/temp.h>
#include <rtgn/tick_clock.h>

#define TICK_NANO_DELAY (1000000000 / 30)

int main()
{
    printf("number: %d\n", rtgn_add(40, 2));
    rtgn_startTickClock(TICK_NANO_DELAY);
    for(;;) {
        if(rtgn_tickClock())
            printf("tick\n");
    }
    return 0;
}
