#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include <rtgn/client.h>
#include <rtgn/tick_clock.h>
#include <rtgn/common_types.h>
#include <rtgn/network_address.h>

#include "./config.h"

typedef struct GameState GameState;
struct GameState
{
    int playerCount;
    int number;
};

void initGameState(GameState* gameState, int playerCount) {
    gameState->playerCount = playerCount;
    gameState->number = 0;
}

void tickGameState(GameState* gameState, rtgn_Input* inputs) {
    if(gameState->playerCount > 0) {
        if(inputs[0].inputFlags & RTGN_INPUT_FLAG_LEFT)
            gameState->number -= 1;
        if(inputs[0].inputFlags & RTGN_INPUT_FLAG_RIGHT)
            gameState->number += 1;
    }
}

int main()
{
    const char* err;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        err = SDL_GetError();
        fprintf(stderr, "failed to init SDL2 '%s'\n", err);
        exit(1);
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    window = SDL_CreateWindow(
        "top down shooter",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        500, 500,
        SDL_WINDOW_SHOWN);
    if(window == NULL) {
        err = SDL_GetError();
        fprintf(stderr, "failed to create SDL2 window '%s'\n", err);
        exit(1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        err = SDL_GetError();
        fprintf(stderr, "failed to create SDL2 renderer '%s'\n", err);
        exit(1);
    }
    
    rtgn_networkAddress_t srvAddr;
    if(rtgn_networkAddress("127.0.0.1", SERVER_PORT, &srvAddr) < 0) {
        fprintf(stderr, "invalid server address string\n");
        exit(1);
    }

    rtgn_Client client;
    rtgn_initClient(
        &client,
        srvAddr,
        malloc(sizeof(GameState)),
        (rtgn_init_game_state_f)initGameState,
        (rtgn_tick_game_state_f)tickGameState);

    rtgn_startTickClock(TICK_NANO_DELAY);
    int quit = 0, i = 0;
    while(quit == 0) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) if (event.type == SDL_QUIT) quit = 1;
        if(rtgn_tickClock()) {
            printf("%d\n", ((GameState*)client.gameState)->number);
            rtgn_Input input = {
                i < 10 ? RTGN_INPUT_FLAG_LEFT : RTGN_INPUT_FLAG_RIGHT
            };
            rtgn_tickClient(&client, &input);
            i++;
        }
    }
    
    rtgn_destroyClient(&client);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
