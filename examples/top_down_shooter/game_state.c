#include "./game_state.h"

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
