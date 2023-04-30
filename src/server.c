#include "rtgn/server.h"

#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h> /* TEMP */

#include "rtgn/config.h"

#include "./utils.h"
#include "./socket.h"
#include "./server_packet_handle.h"

static void acceptTcpConnections(rtgn_Server* server)
{
    rtgn_tcpSrvSocket_t conSocket;
    rtgn_networkAddress_t addr;
    while(tcpSrvSocket_accept(server->tcpSocket, &conSocket, &addr)) {
        int i;
        for(i = 0; i < server->maxConnections; i++)
            if(server->connections[i].state == RTGN_SRV_CON_STATE_NO_CONNECTION) {
                server->connections[i].state = RTGN_SRV_CON_STATE_CONNECTING;
                server->connections[i].networkAddr = addr;
                server->connections[i].tcpSocket = conSocket;
                printf("new connection\n");
                printf("%d\n", addr.sin_port);
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

static void handleTcpPackets(rtgn_Server* server)
{
    ssize_t bytesRead;
    for(rtgn_srvConIndex_t con = 0; con < server->maxConnections; con++)
        while(server->connections[con].state != RTGN_SRV_CON_STATE_NO_CONNECTION) {
            tcpSrvConSocket_read(
                server->connections[con].tcpSocket,
                RTGN_PACKET_BUFFER_SIZE,
                server->packetBuffer,
                &bytesRead);
            if(bytesRead <= 0) break;
            serverHandleTcpPacket(
                server,
                con,
                bytesRead,
                server->packetBuffer);
        }
}

static void handleUdpPackets(rtgn_Server* server)
{
    ssize_t bytesRead;
    rtgn_networkAddress_t addr;
    for(;;) {
        udpSrvSocket_read(
            server->udpSocket,
            RTGN_PACKET_BUFFER_SIZE,
            server->packetBuffer,
            &bytesRead,
            &addr);
        if(bytesRead <= 0) break;
        printf("%d\n", addr.sin_port);
        serverHandleUdpPacket(server, bytesRead, server->packetBuffer);
    }
}

void rtgn_initServer(rtgn_Server* server, int port, int maxConnections)
{
    server->port = port;
    server->tcpSocket = tcpSrvSocket_create(port);
    server->udpSocket = udpSrvSocket_create(port);
    server->packetBuffer = emalloc(RTGN_PACKET_BUFFER_SIZE);
    server->maxConnections = maxConnections;
    server->connections = emalloc(maxConnections * sizeof(rtgn_SrvCon));
    for(int i = 0; i < maxConnections; i++)
        server->connections[i].state = RTGN_SRV_CON_STATE_NO_CONNECTION;
}

void rtgn_tickServer(rtgn_Server* server)
{
    acceptTcpConnections(server);
    handleTcpPackets(server);
    handleUdpPackets(server);
}

void rtgn_destroyServer(rtgn_Server* server)
{
    tcpSrvSocket_close(server->tcpSocket);
    tcpSrvSocket_close(server->udpSocket);
    free(server->packetBuffer);
    free(server->connections);
}
