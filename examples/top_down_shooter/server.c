#include <stdio.h>

#include <rtgn/common_types.h>
#include <rtgn/server.h>
#include <rtgn/tick_clock.h>

#include "./config.h"

int main()
{
    rtgn_Server server;
    rtgn_initServer(&server, SERVER_PORT, 16);
    
    rtgn_startTickClock(TICK_NANO_DELAY);
    for(;;) {
        if(rtgn_tickClock()) {
            rtgn_tickServer(&server);
        }
    }

    rtgn_destroyServer(&server);
}
