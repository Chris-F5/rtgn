#ifndef RTGN_CLIENT_PACKET_HANDLE_H
#define RTGN_CLIENT_PACKET_HANDLE_H

#include "rtgn/common_types.h"
#include "rtgn/client.h"

#include "./packet.h"

void clientHandleTcpPacket(
    rtgn_Client* client,
    size_t packetSize,
    TcpPacket* packet);
void clientHandleUdpPacket(
    rtgn_Client* client,
    size_t packetSize,
    UdpSrvPacket* packet);

#endif
