#include "rtgn/server.h"

#include <stdlib.h>
#include <stdio.h>

#include "rtgn/config.h"

#include "./utils.h"
#include "./socket.h"
#include "./server_packet_handle.h"

static void acceptNewConnections(rtgn_Server* server)
{
    rtgn_tcpSrvSocket_t conSocket;
    while(tcpSrvSocket_accept(server->srvSocket, &conSocket)) {
        int i;
        for(i = 0; i < server->maxConnections; i++)
            if(server->connections[i].state == RTGN_SRV_CON_STATE_NO_CONNECTION) {
                server->connections[i].state = RTGN_SRV_CON_STATE_NEW;
                server->connections[i].socket = conSocket;
                printf("new connection\n");
                break;
            }
        if(i == server->maxConnections) {
            warn("client tried to connect to full server\n");
            // TODO: send a 'server full' packet
            tcpSrvConSocket_close(conSocket);
            continue;
        }
    }
}

static void handleConnectionPackets(rtgn_Server* server)
{
    ssize_t bytesRead;
    for(rtgn_srvConIndex_t con = 0; con < server->maxConnections; con++)
        while(server->connections[con].state != RTGN_SRV_CON_STATE_NO_CONNECTION) {
            tcpSrvConSocket_read(
                server->connections[con].socket,
                RTGN_TCP_SERVER_PACKET_BUFFER_SIZE,
                server->packetBuffer,
                &bytesRead);
            if(bytesRead <= 0) break;
            serverHandleTcpPacket(
                server,
                con,
                bytesRead,
                (TcpPacket*)server->packetBuffer);
        }
}

void rtgn_initServer(rtgn_Server* server, int port, int maxConnections)
{
    server->port = port;
    server->srvSocket = tcpSrvSocket_create(port);
    server->packetBuffer = emalloc(RTGN_TCP_SERVER_PACKET_BUFFER_SIZE);
    server->maxConnections = maxConnections;
    server->connections = emalloc(maxConnections * sizeof(rtgn_SrvCon));
    for(int i = 0; i < maxConnections; i++)
        server->connections[i].state = RTGN_SRV_CON_STATE_NO_CONNECTION;
}

void rtgn_tickServer(rtgn_Server* server)
{
    acceptNewConnections(server);
    handleConnectionPackets(server);
}

void rtgn_destroyServer(rtgn_Server* server)
{
    tcpSrvSocket_close(server->srvSocket);
    free(server->packetBuffer);
    free(server->connections);
}
