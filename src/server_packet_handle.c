#include "./server_packet_handle.h"

#include <stdio.h>

#include "./utils.h"
#include "./socket.h"

#define TCP_PACKET_HANDLE_PARAMS \
    rtgn_Server* server, \
    rtgn_srvConIndex_t conIndex, \
    size_t packetSize, \
    TcpPacket* packet

void handleGreet(TCP_PACKET_HANDLE_PARAMS)
{
    TcpPacket_GreetServer* greet;
    greet = (TcpPacket_GreetServer*)packet;

    if(packetSize < sizeof(TcpPacket_GreetServer)) {
        warn("received 'greet server' packet with size too small");
        return;
    }
    if(packetSize != sizeof(TcpPacket_GreetServer) + greet->nameSize) {
        warn("received 'greet server' packet with invalid size");
        return;
    }
    greet->name[greet->nameSize - 1] = 0;
    printf("'%s' greeted the server!\n", greet->name);

    TcpPacket_GreetClient res;
    res.type = TCP_PACKET_TYPE_GREET_CLIENT;
    res.conIndex = conIndex;

    tcpSrvConSocket_write(
        server->connections[conIndex].tcpSocket,
        sizeof(res),
        &res);
}

void (*tcpPacketHandlers[])(TCP_PACKET_HANDLE_PARAMS) = {
    [TCP_PACKET_TYPE_GREET_SERVER] = handleGreet,
};

void serverHandleTcpPacket(
    rtgn_Server* server,
    rtgn_srvConIndex_t conIndex,
    size_t packetSize,
    TcpPacket* packet)
{
    if(packetSize < sizeof(TcpPacket)) {
        warn("received packet too small");
        return;
    }
    if(packet->type < 0) {
        warn("received packet with negative type");
        return;
    }
    if(packet->type > sizeof(tcpPacketHandlers) / sizeof(tcpPacketHandlers[0])) {
        warn("receied packet with type larger then expected");
        return;
    }
    void (*handler)(TCP_PACKET_HANDLE_PARAMS);
    handler = tcpPacketHandlers[packet->type];
    if(handler == NULL) {
        warn("received packet with unexpected type");
        return;
    }
    handler(server, conIndex, packetSize, packet);
}

#define UDP_PACKET_HANDLE_PARAMS \
    rtgn_Server* server, \
    rtgn_srvConIndex_t conIndex, \
    size_t packetSize, \
    UdpCliPacket* packet

void handleTest(UDP_PACKET_HANDLE_PARAMS)
{
    UdpCliPacket_Test* test;
    test = (UdpCliPacket_Test*)packet;

    if(packetSize != sizeof(UdpCliPacket_Test)) {
        warn("received udp client test packet with invalid size");
        return;
    }

    printf(
        "received udp client test packet; number: %d; con: %d\n",
        test->number,
        test->conIndex);

    UdpSrvPacket_Test res;
    res.type = UDP_SRV_PACKET_TYPE_TEST;
    res.number = 55;
    udpSrvSocket_write(
        server->udpSocket,
        server->connections[test->conIndex].networkAddr,
        sizeof(res),
        &res);
}

void (*udpPacketHandlers[])(UDP_PACKET_HANDLE_PARAMS) = {
    [UDP_CLI_PACKET_TYPE_TEST] = handleTest,
};

void serverHandleUdpPacket(
    rtgn_Server* server,
    size_t packetSize,
    UdpCliPacket* packet)
{
    if(packetSize < sizeof(UdpCliPacket)) {
        warn("received udp client packet that is too small");
        return;
    }
    if(packet->type < 0) {
        warn("received udp client packet with negative type");
        return;
    }
    if(packet->type > sizeof(udpPacketHandlers) / sizeof(udpPacketHandlers[0])) {
        warn("received udp client packet with type larget than expected");
        return;
    }
    void (*handler)(UDP_PACKET_HANDLE_PARAMS);
    handler = udpPacketHandlers[packet->type];
    if(handler == NULL) {
        warn("received udp client packet with unexpected type");
        return;
    }
    if(packet->conIndex < 0) {
        warn("reveived udp client packet with negative connection index");
        return;
    }
    if (packet->conIndex >= server->maxConnections) {
        warn("received udp client packet with invalid connection index (out of \
bounds)");
        return;
    }
    if(server->connections[packet->conIndex].state 
        == RTGN_SRV_CON_STATE_NO_CONNECTION) {
      warn("received udp client packet with invalid connection index (no \
connection)");
      return;
    }
    handler(server, packet->conIndex, packetSize, packet);
}
