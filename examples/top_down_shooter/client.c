#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include <rtgn/client.h>
#include <rtgn/tick_clock.h>
#include <rtgn/common_types.h>

#include "./config.h"
#include "./game_state.h"
#include "./client_ctx.h"
#include "./window_event_handler.h"

int main()
{
    const char* err;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        err = SDL_GetError();
        fprintf(stderr, "failed to init SDL2 '%s'\n", err);
        exit(1);
    }

    ClientCtx clientCtx;
    initClientCtx(&clientCtx);

    rtgn_startTickClock(TICK_NANO_DELAY);
    int i = 0;
    while(clientCtx.quit == 0) {
        SDL_Event event;
        while(SDL_PollEvent(&event))
            handleWindowEvent(&clientCtx, &event);
        if(rtgn_tickClock()) {
            rtgn_tickClient(&clientCtx.networkClient, &clientCtx.currentInput);
            printf("%d\n", ((GameState*)clientCtx.networkClient.gameState)->number);
            i++;
        }
    }

    destroyClientCtx(&clientCtx);
    SDL_Quit();

    return 0;
}
