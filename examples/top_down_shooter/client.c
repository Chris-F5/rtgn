#include <stdio.h>
#include <stdlib.h>

#include <rtgn/client.h>
#include <rtgn/tick_clock.h>

#define TICK_NANO_DELAY (1000000000 / 2)

typedef struct GameState GameState;
struct GameState
{
    int number;
};

void initGameState(GameState* gameState) {
    gameState->number = 1;
}

void tickGameState(GameState* gameState) {
    gameState->number = gameState->number * 2;
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
    for(;;) {
        if(rtgn_tickClock()) {
            printf("%d\n", ((GameState*)client.gameState)->number);
            rtgn_tickClient(&client);
        }
    }
    return 0;
}
