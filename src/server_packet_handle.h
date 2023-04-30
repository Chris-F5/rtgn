#ifndef RTGN_SERVER_PACKET_HANDLE_H
#define RTGN_SERVER_PACKET_HANDLE_H

#include "rtgn/common_types.h"
#include "rtgn/server.h"

#include "./packet.h"

void serverHandleTcpPacket(
    rtgn_Server* server,
    rtgn_srvConIndex_t conIndex,
    size_t packetSize,
    TcpPacket* packet);

void serverHandleUdpPacket(
    rtgn_Server* server,
    size_t packetSize,
    UdpCliPacket* packet);

#endif
