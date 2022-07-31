#include <stdio.h>
#include <stdlib.h>

#include <rtgn/client.h>
#include <rtgn/tick_clock.h>
#include <rtgn/common_types.h>

#define TICK_NANO_DELAY (1000000000 / 2)

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
    rtgn_Client client;
    rtgn_initClient(
        &client,
        malloc(sizeof(GameState)),
        (rtgn_init_game_state_f)initGameState,
        (rtgn_tick_game_state_f)tickGameState);

    rtgn_startTickClock(TICK_NANO_DELAY);
    uint32_t i = 0;
    for(;;) {
        if(rtgn_tickClock()) {
            printf("%d\n", ((GameState*)client.gameState)->number);
            rtgn_Input input = {
                i < 10 ? RTGN_INPUT_FLAG_LEFT : RTGN_INPUT_FLAG_RIGHT
            };
            rtgn_tickClient(&client, &input);
            i++;
        }
    }
    return 0;
}
