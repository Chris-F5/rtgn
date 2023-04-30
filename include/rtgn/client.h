#ifndef RTGN_CLIENT_H
#define RTGN_CLIENT_H

#include "./common_types.h"

#define RTGN_CLIENT_STATE_NEW 0
#define RTGN_CLIENT_STATE_CONNECTED 1

typedef uint32_t rtgn_clientState_t;

typedef struct rtgn_Client rtgn_Client;
struct rtgn_Client
{
    rtgn_clientState_t state;
    rtgn_srvConIndex_t conIndex;
    rtgn_networkAddress_t srvAddr;
    rtgn_tcpCliSocket_t tcpSocket;
    rtgn_udpCliSocket_t udpSocket;
    rtgn_game_state_t* gameState;
    rtgn_init_game_state_f initGameState;
    rtgn_tick_game_state_f tickGameState;
    void* packetBuffer;
};

void rtgn_initClient(
    rtgn_Client* client,
    rtgn_networkAddress_t srvAddr,
    rtgn_game_state_t* gameStateMemory,
    rtgn_init_game_state_f initGameState,
    rtgn_tick_game_state_f tickGameState);
void rtgn_tickClient(rtgn_Client* client, rtgn_Input* input);
void rtgn_destroyClient(rtgn_Client* client);

#endif
