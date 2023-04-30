#include "./client_packet_handle.h"

#include <stdio.h>

#include "./utils.h"
#include "./socket.h"

/* TCP */

#define TCP_HANDLER_PARAMS rtgn_Client* client, size_t packetSize, TcpPacket* packet

void handleGreet(TCP_HANDLER_PARAMS)
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

    UdpCliPacket_Test testRes;
    testRes.type = UDP_CLI_PACKET_TYPE_TEST;
    testRes.conIndex = client->conIndex;
    testRes.number = 42;

    udpCliSocket_write(
        client->udpSocket,
        client->srvAddr,
        sizeof(testRes),
        &testRes);
}

void (*tcpHandlers[])(TCP_HANDLER_PARAMS) = {
    [TCP_PACKET_TYPE_GREET_CLIENT] = handleGreet,
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
    void (*handler)(TCP_HANDLER_PARAMS);
    handler = tcpHandlers[packet->type];
    if(handler == NULL) {
        warn("received packet with unexpected type");
        return;
    }
    handler(client, packetSize, packet);
}

/* UDP */

#define UDP_HANDLER_PARAMS rtgn_Client* client, size_t packetSize, UdpSrvPacket* packet

void handleUdpTest(UDP_HANDLER_PARAMS)
{
    UdpSrvPacket_Test* test;
    test = (UdpSrvPacket_Test*)packet;

    if(packetSize != sizeof(UdpSrvPacket_Test)) {
        warn("received udp test packet with invalid size");
        return;
    }
    
    printf("received test packet; number: %d\n", test->number);
}

void (*udpHandlers[])(UDP_HANDLER_PARAMS) = {
    [UDP_SRV_PACKET_TYPE_TEST] = handleUdpTest
};

void clientHandleUdpPacket(
    rtgn_Client* client,
    size_t packetSize,
    UdpSrvPacket* packet)
{
    if(packetSize < sizeof(UdpSrvPacket)) {
        warn("received udp packet which is too small");
        return;
    }
    if(packet->type < 0) {
        warn("received udp packet with negative type");
        return;
    }
    if(packet->type > sizeof(udpHandlers) / sizeof(udpHandlers[0])) {
        warn("received udp packet with type larget than expected");
        return;
    }
    void (*handler)(UDP_HANDLER_PARAMS);
    handler = udpHandlers[packet->type];
    if(handler == NULL) {
        warn("received udp packet with unexpected type");
        return;
    }
    handler(client, packetSize, packet);
}
