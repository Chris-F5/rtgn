#ifndef RTGN_SERVER_H
#define RTGN_SERVER_H

#include <inttypes.h>

#include "./common_types.h"

#define RTGN_SRV_CON_STATE_NO_CONNECTION 0
#define RTGN_SRV_CON_STATE_NEW 1
#define RTGN_SRV_CON_STATE_IDLE 2

typedef uint32_t rtgn_srvConState_t;

typedef struct rtgn_SrvCon rtgn_SrvCon;
struct rtgn_SrvCon
{
    rtgn_srvConState_t state;
    rtgn_tcpSrvConSocket_t socket;
};

typedef struct rtgn_Server rtgn_Server;
struct rtgn_Server
{
    int port;
    rtgn_tcpSrvSocket_t srvSocket;
    void* packetBuffer;
    int maxConnections;
    rtgn_SrvCon* connections;
};

void rtgn_initServer(rtgn_Server* server, int port, int maxConnections);
void rtgn_tickServer(rtgn_Server* server);
void rtgn_destroyServer(rtgn_Server* server);

#endif
