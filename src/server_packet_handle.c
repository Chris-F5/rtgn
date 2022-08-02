#include "./server_packet_handle.h"

#include <stdio.h>

#include "./utils.h"

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
}

void (*tcpPacketHandlers[])(TCP_PACKET_HANDLE_PARAMS) = {
    [TCP_PACKET_TYPE_GREET_SERVER] = handleGreet,
};

void handleTcpPacket(
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
