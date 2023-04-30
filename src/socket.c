#include "./socket.h"

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "rtgn/config.h"
#include "./utils.h"

/* TCP SERVER SOCKET */

rtgn_tcpSrvSocket_t tcpSrvSocket_create(int port)
{
    rtgn_tcpSrvSocket_t sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
        die("failed to create tcp server socket '%s'", strerror(errno));
    if(fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) < 0)
        die("failed to set nonblock flag on tcp server socket '%s'", strerror(errno));

    struct sockaddr_in srvAddr;
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvAddr.sin_port = htons(port);
    bzero(srvAddr.sin_zero, sizeof(srvAddr.sin_zero));

    if(bind(sock, (struct sockaddr*)&srvAddr, sizeof(srvAddr)) < 0)
        die("failed to bind tcp server socket '%s'", strerror(errno));

    if(listen(sock, RTGN_TCP_SERVER_QUEUE_REQUESTS) < 0)
        die("failed to listen on tcp server socket '%s'", strerror(errno));

    return sock;
}

int tcpSrvSocket_accept(
    rtgn_tcpSrvSocket_t srvSock,
    rtgn_tcpSrvConSocket_t* con,
    rtgn_networkAddress_t* addr)
{
    socklen_t addrLen;
    *con = accept(srvSock, (struct sockaddr*)addr, &addrLen);
    if(*con < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
        die("failed to accept connection from tcp server socket '%s'", strerror(errno));
    if(*con < 0) return 0;
    if(fcntl(*con, F_SETFL, fcntl(*con, F_GETFL) | O_NONBLOCK) < 0)
        die("failed to set nonblock flag on tcp server con socket '%s'", strerror(errno));
    if(addrLen != sizeof(*addr) || addr->sin_family != AF_INET)
        die("failed to read udp server socket, invalid address");
    return 1;
}

void tcpSrvSocket_close(rtgn_tcpSrvSocket_t sock)
{
    if(close(sock) < 0)
        die("failed to close tcp server socket '%s'", strerror(errno));
}

/* TCP SERVER CONNECTION SOCKET */

void tcpSrvConSocket_read(
    rtgn_tcpSrvConSocket_t con,
    size_t bufferSize,
    void* buffer,
    ssize_t* bytesRead)
{
    *bytesRead = read(con, buffer, bufferSize);
    if(bytesRead < 0) {
        if(errno != EAGAIN && errno != EWOULDBLOCK)
            die("failed to read bytes from tcp server cnnection socket '%s'", strerror(errno));
        *bytesRead = 0;
    }
}

void tcpSrvConSocket_write(
    rtgn_tcpSrvConSocket_t con,
    size_t bufferSize,
    const void* buffer)
{
    ssize_t bytesWritten = write(con, buffer, bufferSize);
    if(bytesWritten < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
        die("write to tcp server connection socket was blocked '%s'", strerror(errno));
    if(bytesWritten < 0)
        die("failed to write to tcp server connection socket '%s'", strerror(errno));
    if(bytesWritten != bufferSize)
        die("failed to write full buffer to tcp server connection socket");
}

void tcpSrvConSocket_close(rtgn_tcpSrvConSocket_t sock)
{
    if(close(sock) < 0)
        die("failed to close tcp server connection socket '%s'", strerror(errno));
}

/* TCP CLIENT SOCKET */

rtgn_tcpCliSocket_t tcpCliSocket_create(rtgn_networkAddress_t srvAddr)
{
    rtgn_tcpCliSocket_t sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
        die("failed to create tcp client socket '%s'", strerror(errno));
    if(fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) < 0)
        die("failed to set nonblock flag on tcp server socket '%s'", strerror(errno));

    if(connect(sock, (struct sockaddr*)&srvAddr, sizeof(srvAddr)) < 0 && errno != EINPROGRESS)
        die("tcp client socket failed to connect to server '%s'", strerror(errno));

    return sock;
}

void tcpCliSocket_read(
    rtgn_tcpCliSocket_t sock,
    size_t bufferSize,
    void* buffer,
    ssize_t* bytesRead)
{
    *bytesRead = read(sock, buffer, bufferSize);
    if(bytesRead < 0) {
        if(errno != EAGAIN && errno != EWOULDBLOCK)
            die("failed to read bytes from tcp client socket '%s'", strerror(errno));
        *bytesRead = 0;
    }
}

void tcpCliSocket_write(
    rtgn_tcpCliSocket_t sock,
    size_t bufferSize,
    const void* buffer)
{
    ssize_t bytesWritten = write(sock, buffer, bufferSize);
    if(bytesWritten < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
        die("write to tcp client socket was blocked");
    if(bytesWritten < 0)
        die("failed to write to tcp client socket");
    if(bytesWritten != bufferSize)
        die("failed to write full buffer to tcp client socket");
}

void tcpCliSocket_close(rtgn_tcpCliSocket_t sock)
{
    if(close(sock) < 0)
        die("failed to close tcp client socket '%s'", strerror(errno));
}

/* UDP SERVER SOCKET */

rtgn_udpSrvSocket_t udpSrvSocket_create(int port)
{
    int sock;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
        die("failed to create udp server socket '%s'", strerror(errno));
    if(fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) < 0)
        die(
            "failed to set nonblock flag on udp server socket '%s'",
            strerror(errno));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    bzero(&addr.sin_zero, sizeof(addr.sin_zero));

    if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        die("failed to bind udp server socket '%s'", strerror(errno));

    return sock;
}

void udpSrvSocket_read(
    rtgn_udpSrvSocket_t sock,
    size_t bufferSize,
    void* buffer,
    ssize_t* bytesRead,
    rtgn_networkAddress_t* cliAddr)
{
    socklen_t addrLen;
    addrLen = sizeof(struct sockaddr);
    *bytesRead = recvfrom(
        sock,
        buffer,
        bufferSize,
        0,
        (struct sockaddr*)cliAddr,
        &addrLen);

    if(*bytesRead < 0) {
        if(errno != EAGAIN && errno != EWOULDBLOCK)
            die("failed to read udp server socket '%s'", strerror(errno));
        *bytesRead = 0;
        return;
    }

    if(addrLen != sizeof(*cliAddr) || cliAddr->sin_family != AF_INET)
        die("failed to read udp server socket, invalid address");
}

void udpSrvSocket_write(
    rtgn_udpSrvSocket_t sock,
    rtgn_networkAddress_t cliAddr,
    size_t bufferSize,
    const void* buffer)
{
    ssize_t bytesSent;
    bytesSent = sendto(
        sock,
        buffer,
        bufferSize,
        0,
        (struct sockaddr*)&cliAddr,
        sizeof(cliAddr));

    if(bytesSent != bufferSize)
        die("failed to write udp server socket '%s'", strerror(errno));
}

void udpSrvSocket_close(rtgn_udpSrvSocket_t sock)
{
    if(close(sock) < 0)
        die("failed to close udp server socket '%s'", strerror(errno));
}

/* UDP CLIENT SOCKET */

rtgn_udpCliSocket_t udpCliSocket_create(void)
{
    int sock;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
        die("failed to create udp client socket '%s'", strerror(errno));
    if(fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) < 0)
        die(
            "failed to set nonblock flag on udp client socket '%s'",
            strerror(errno));

    return sock;
}

void udpCliSocket_read(
    rtgn_udpCliSocket_t sock,
    size_t bufferSize,
    void* buffer,
    ssize_t* bytesRead,
    rtgn_networkAddress_t* srvAddr)
{
    socklen_t addrLen;
    addrLen = sizeof(struct sockaddr);
    *bytesRead = recvfrom(
        sock,
        buffer,
        bufferSize,
        0,
        (struct sockaddr*)srvAddr,
        &addrLen);

    if(*bytesRead < 0) {
        if(errno != EAGAIN && errno != EWOULDBLOCK)
            die("failed to read udp client socket '%s'", strerror(errno));
        *bytesRead = 0;
        return;
    }

    if(addrLen != sizeof(*srvAddr) || srvAddr->sin_family != AF_INET)
        die("failed to read udp client socket, invalid address");
}

void udpCliSocket_write(
    rtgn_udpCliSocket_t sock,
    rtgn_networkAddress_t srvAddr,
    size_t bufferSize,
    const void* buffer)
{
    ssize_t bytesSent;
    bytesSent = sendto(
        sock,
        buffer,
        bufferSize,
        0,
        (struct sockaddr*)&srvAddr,
        sizeof(srvAddr));

    if(bytesSent != bufferSize)
        die("failed to write udp client socket '%s'", strerror(errno));
}

void udpCliSocket_close(rtgn_udpCliSocket_t sock)
{
    if(close(sock) < 0)
        die("failed to close udp client socket '%s'", strerror(errno));
}
