#ifndef RTGN_PACKET_H
#define RTGN_PACKET_H

#include <inttypes.h>

#include "rtgn/common_types.h"

/* TCP */

#define TCP_PACKET_TYPE_GREET_CLIENT 0
#define TCP_PACKET_TYPE_GREET_SERVER 1

typedef uint8_t tcpPacketType_t;

typedef struct TcpPacket TcpPacket;
struct TcpPacket
{
    tcpPacketType_t type;
    char data[];
};

typedef struct TcpPacket_GreetServer TcpPacket_GreetServer;
struct TcpPacket_GreetServer
{
    tcpPacketType_t type;
    uint8_t nameSize;
    char name[];
};

struct TcpPacket_GreetClient
{
    tcpPacketType_t type;
    uint32_t number;
};

#endif
