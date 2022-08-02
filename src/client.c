#include "rtgn/client.h"

#include "./socket.h"
#include "./packet.h"
#include "./utils.h"

#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void rtgn_initClient(
    rtgn_Client* client,
    rtgn_networkAddress_t srvAddr,
    rtgn_game_state_t* gameStateMemory,
    rtgn_init_game_state_f initGameState,
    rtgn_tick_game_state_f tickGameState)
{
    client->socket = tcpClientSocket_create(srvAddr);

    size_t packetSize = sizeof(TcpPacket_GreetServer) + strlen("chris") + 1;
    TcpPacket_GreetServer* greetPacket = emalloc(packetSize);
    greetPacket->type = TCP_PACKET_TYPE_GREET_SERVER;
    greetPacket->nameSize = strlen("chris") + 1;
    strcpy(greetPacket->name, "chris");
    tcpClientSocket_write(client->socket, packetSize, greetPacket);
    free(greetPacket);

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
