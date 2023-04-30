#include "rtgn/client.h"

#include "rtgn/config.h"

#include "./socket.h"
#include "./packet.h"
#include "./utils.h"
#include "./client_packet_handle.h"

#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void handleTcpPackets(rtgn_Client* client)
{
    ssize_t bytesRead;
    for(;;) {
        tcpCliSocket_read(
            client->tcpSocket,
            RTGN_PACKET_BUFFER_SIZE,
            client->packetBuffer,
            &bytesRead);
        if(bytesRead <= 0) break;
        clientHandleTcpPacket(client, bytesRead, (TcpPacket*)client->packetBuffer);
    }
}

static void handleUdpPackets(rtgn_Client* client)
{
    rtgn_networkAddress_t addr;
    ssize_t bytesRead;
    for(;;) {
        udpCliSocket_read(
            client->udpSocket,
            RTGN_PACKET_BUFFER_SIZE,
            client->packetBuffer,
            &bytesRead,
            &addr);
        if(bytesRead <= 0) break;
        printf("udp\n");
        clientHandleUdpPacket(client, bytesRead, client->packetBuffer);
    }
}

void rtgn_initClient(
    rtgn_Client* client,
    rtgn_networkAddress_t srvAddr,
    rtgn_game_state_t* gameStateMemory,
    rtgn_init_game_state_f initGameState,
    rtgn_tick_game_state_f tickGameState)
{
    client->state = RTGN_CLIENT_STATE_NEW;
    client->srvAddr = srvAddr;
    client->tcpSocket = tcpCliSocket_create(client->srvAddr);
    client->udpSocket = udpCliSocket_create();
    client->conIndex = 0;

    client->packetBuffer = emalloc(RTGN_PACKET_BUFFER_SIZE);

    size_t packetSize = sizeof(TcpPacket_GreetServer) + strlen("chris") + 1;
    TcpPacket_GreetServer* greetPacket = emalloc(packetSize);
    greetPacket->type = TCP_PACKET_TYPE_GREET_SERVER;
    greetPacket->nameSize = strlen("chris") + 1;
    strcpy(greetPacket->name, "chris");
    tcpCliSocket_write(client->tcpSocket, packetSize, greetPacket);
    free(greetPacket);

    client->gameState = gameStateMemory;
    client->initGameState = initGameState;
    client->tickGameState = tickGameState;
    client->initGameState(client->gameState, 1);
}

void rtgn_tickClient(rtgn_Client* client, rtgn_Input* input)
{
    handleTcpPackets(client);
    handleUdpPackets(client);
    client->tickGameState(client->gameState, input);
}

void rtgn_destroyClient(rtgn_Client* client)
{
    tcpCliSocket_close(client->tcpSocket);
    udpCliSocket_close(client->udpSocket);
    free(client->packetBuffer);
}
