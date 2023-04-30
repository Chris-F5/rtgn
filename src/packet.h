#ifndef RTGN_PACKET_H
#define RTGN_PACKET_H

#include <inttypes.h>

#include "rtgn/common_types.h"

/* TCP */

#define TCP_PACKET_TYPE_GREET_CLIENT 0
#define TCP_PACKET_TYPE_GREET_SERVER 1

typedef uint8_t tcpPacketType_t;
typedef uint8_t udpPacketType_t;

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

typedef struct TcpPacket_GreetClient TcpPacket_GreetClient;
struct TcpPacket_GreetClient
{
    tcpPacketType_t type;
    rtgn_srvConIndex_t conIndex;
};

/* UDP SERVER */

#define UDP_SRV_PACKET_TYPE_TEST 0

typedef struct UdpSrvPacket UdpSrvPacket;
struct UdpSrvPacket
{
    udpPacketType_t type;
    char data[];
};

typedef struct UdpSrvPacket_Test UdpSrvPacket_Test;
struct UdpSrvPacket_Test
{
    udpPacketType_t type;
    uint32_t number;
};

/* UDP CLIENT */

#define UDP_CLI_PACKET_TYPE_TEST 0

typedef struct UdpCliPacket UdpCliPacket;
struct UdpCliPacket
{
    udpPacketType_t type;
    rtgn_srvConIndex_t conIndex;
    char data[];
};

typedef struct UdpCliPacket_Test UdpCliPacket_Test;
struct UdpCliPacket_Test
{
    udpPacketType_t type;
    rtgn_srvConIndex_t conIndex;
    uint32_t number;
};

#endif
