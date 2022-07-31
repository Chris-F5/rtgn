#ifndef RTGN_CLIENT_H
#define RTGN_CLIENT_H

#include "./common_types.h"

typedef struct rtgn_Client rtgn_Client;
struct rtgn_Client
{
    rtgn_game_state_t* gameState;
    rtgn_init_game_state_f initGameState;
    rtgn_tick_game_state_f tickGameState;
};

void rtgn_initClient(
    rtgn_Client* client,
    rtgn_game_state_t* gameStateMemory,
    rtgn_init_game_state_f initGameState,
    rtgn_tick_game_state_f tickGameState);

void rtgn_tickClient(rtgn_Client* client, rtgn_Input* input);

#endif
