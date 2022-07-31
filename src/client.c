#include "rtgn/client.h"

void rtgn_initClient(
    rtgn_Client* client,
    rtgn_game_state_t* gameStateMemory,
    rtgn_init_game_state_f initGameState,
    rtgn_tick_game_state_f tickGameState)
{
    client->gameState = gameStateMemory;
    client->initGameState = initGameState;
    client->tickGameState = tickGameState;
    client->initGameState(client->gameState);
}

void rtgn_tickClient(rtgn_Client* client)
{
    client->tickGameState(client->gameState);
}
