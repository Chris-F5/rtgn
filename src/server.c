#include "rtgn/server.h"

#include <stdlib.h>
#include <stdio.h>

#include "./utils.h"
#include "./socket.h"

static void acceptNewConnections(rtgn_Server* server)
{
    rtgn_tcpSrvSocket_t conSocket;
    while(tcpSrvSocket_accept(server->srvSocket, &conSocket)) {
        int i;
        for(i = 0; i < server->maxConnections; i++)
            if(server->connections[i].state == RTGN_SRV_CON_STATE_EMPTY) {
                server->connections[i].state = RTGN_SRV_CON_STATE_NEW;
                server->connections[i].socket = conSocket;
                printf("new connection\n");
                break;
            }
        if(i == server->maxConnections) {
            fprintf(stderr, "client tried to connect to full server\n");
            // TODO: send a 'server full' packet
            tcpSrvConSocket_close(conSocket);
        }
    }
}

void rtgn_initServer(rtgn_Server* server, int port, int maxConnections)
{
    server->port = port;
    server->srvSocket = tcpSrvSocket_create(port);
    server->maxConnections = maxConnections;
    server->connections = emalloc(maxConnections * sizeof(rtgn_SrvCon));
    for(int i = 0; i < maxConnections; i++)
        server->connections[i].state = RTGN_SRV_CON_STATE_EMPTY;
}

void rtgn_tickServer(rtgn_Server* server)
{
    acceptNewConnections(server);
}

void rtgn_destroyServer(rtgn_Server* server)
{
    tcpSrvSocket_close(server->srvSocket);
    free(server->connections);
}
