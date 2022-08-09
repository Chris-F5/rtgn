#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <rtgn/common_types.h>

typedef struct GameState GameState;
struct GameState
{
    int playerCount;
    int number;
};

void initGameState(GameState* gameState, int playerCount);
void tickGameState(GameState* gameState, rtgn_Input* inputs);

#endif
