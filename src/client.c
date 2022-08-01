#include "rtgn/client.h"

#include "./socket.h"

#include <sys/socket.h>

void rtgn_initClient(
    rtgn_Client* client,
    rtgn_networkAddress_t srvAddr,
    rtgn_game_state_t* gameStateMemory,
    rtgn_init_game_state_f initGameState,
    rtgn_tick_game_state_f tickGameState)
{
    tcpClientSocket_create(srvAddr);
    client->gameState = gameStateMemory;
    client->initGameState = initGameState;
    client->tickGameState = tickGameState;
    client->initGameState(client->gameState, 1);
}

void rtgn_tickClient(rtgn_Client* client, rtgn_Input* input)
{
    client->tickGameState(client->gameState, input);
}

void rtgn_destroyClient(rtgn_Client* client)
{
    tcpClientSocket_close(client->socket);
}
