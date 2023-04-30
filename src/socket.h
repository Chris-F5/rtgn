#ifndef RTGN_SOCKET_H
#define RTGN_SOCKET_H

#include <aio.h>

#include "rtgn/common_types.h"

/* TCP SOCKETS */

rtgn_tcpSrvSocket_t tcpSrvSocket_create(int port);
int tcpSrvSocket_accept(
    rtgn_tcpSrvSocket_t srvSock,
    rtgn_tcpSrvConSocket_t* con,
    rtgn_networkAddress_t* addr);
void tcpSrvSocket_close(rtgn_tcpSrvSocket_t sock);

void tcpSrvConSocket_read(
    rtgn_tcpSrvConSocket_t con,
    size_t bufferSize,
    void* buffer,
    ssize_t* bytesRead);
void tcpSrvConSocket_write(
    rtgn_tcpSrvConSocket_t con,
    size_t bufferSize,
    const void* buffer);
void tcpSrvConSocket_close(rtgn_tcpSrvConSocket_t sock);

rtgn_tcpCliSocket_t tcpCliSocket_create(rtgn_networkAddress_t srvAddr);
void tcpCliSocket_read(
    rtgn_tcpCliSocket_t sock,
    size_t bufferSize,
    void* buffer,
    ssize_t* bytesRead);
void tcpCliSocket_write(
    rtgn_tcpCliSocket_t sock,
    size_t bufferSize,
    const void* buffer);
void tcpCliSocket_close(rtgn_tcpCliSocket_t sock);

/* UDP SOCKETS */

rtgn_udpSrvSocket_t udpSrvSocket_create(int port);
void udpSrvSocket_read(
    rtgn_udpSrvSocket_t sock,
    size_t bufferSize,
    void* buffer,
    ssize_t* bytesRead,
    rtgn_networkAddress_t* cliAddr);
void udpSrvSocket_write(
    rtgn_udpSrvSocket_t sock,
    rtgn_networkAddress_t cliAddr,
    size_t bufferSize,
    const void* buffer);
void udpSrvSocket_close(rtgn_udpSrvSocket_t sock);

rtgn_udpCliSocket_t udpCliSocket_create(void);
void udpCliSocket_read(
    rtgn_udpCliSocket_t sock,
    size_t bufferSize,
    void* buffer,
    ssize_t* bytesRead,
    rtgn_networkAddress_t* srvAddr);
void udpCliSocket_write(
    rtgn_udpCliSocket_t sock,
    rtgn_networkAddress_t srvAddr,
    size_t bufferSize,
    const void* buffer);
void udpCliSocket_close(rtgn_udpCliSocket_t sock);

#endif
