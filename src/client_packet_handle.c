#include "./client_packet_handle.h"

#include <stdio.h>

#include "./utils.h"
#include "./socket.h"

#define HANDLER_PARAMS rtgn_Client* client, size_t packetSize, TcpPacket* packet

void handleGreetClient(HANDLER_PARAMS)
{
    TcpPacket_GreetClient* greet;
    greet = (TcpPacket_GreetClient*)packet;

    if(packetSize != sizeof(TcpPacket_GreetClient)) {
        warn("received 'greet client' packet with invalid size");
        return;
    }

    client->state = RTGN_CLIENT_STATE_CONNECTED;
    client->conIndex = greet->conIndex;
    printf("server greeted '%d'\n", greet->conIndex);

    UdpClientPacket_Test testRes;
    testRes.type = UDP_CLIENT_PACKET_TYPE_TEST;
    testRes.conIndex = client->conIndex;
    testRes.number = 42;

    udpClientSocket_write(
        client->udpSocket,
        client->srvAddr,
        sizeof(testRes),
        &testRes);
}

void (*tcpHandlers[])(HANDLER_PARAMS) = {
    [TCP_PACKET_TYPE_GREET_CLIENT] = handleGreetClient,
};

void clientHandleTcpPacket(rtgn_Client* client, size_t packetSize, TcpPacket* packet)
{
    if(packetSize < sizeof(TcpPacket)) {
        warn("received packet too small");
        return;
    }
    if(packet->type < 0) {
        warn("received packet with negative type");
        return;
    }
    if(packet->type > sizeof(tcpHandlers) / sizeof(tcpHandlers[0])) {
        warn("receied packet with type larger then expected");
        return;
    }
    void (*handler)(HANDLER_PARAMS);
    handler = tcpHandlers[packet->type];
    if(handler == NULL) {
        warn("received packet with unexpected type");
        return;
    }
    handler(client, packetSize, packet);
}
