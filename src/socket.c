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
    rtgn_tcpSrvConSocket_t* con)
{
    *con = accept(srvSock, NULL, NULL);
    if(*con < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
        die("failed to accept connection from tcp server socket '%s'", strerror(errno));
    if(*con < 0) return 0;
    if(fcntl(*con, F_SETFL, fcntl(*con, F_GETFL) | O_NONBLOCK) < 0)
        die("failed to set nonblock flag on tcp server con socket '%s'", strerror(errno));
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
    if(bytesRead < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
        die("failed to read bytes from tcp server cnnection socket '%s'", strerror(errno));
}

void tcpSrvConSocket_write(
    rtgn_tcpSrvConSocket_t con,
    size_t bufferSize,
    void* buffer)
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

rtgn_tcpClientSocket_t tcpClientSocket_create(rtgn_networkAddress_t srvAddr)
{
    rtgn_tcpClientSocket_t sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
        die("failed to create tcp client socket '%s'", strerror(errno));
    if(fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) < 0)
        die("failed to set nonblock flag on tcp server socket '%s'", strerror(errno));

    if(connect(sock, (struct sockaddr*)&srvAddr, sizeof(srvAddr)) < 0 && errno != EINPROGRESS)
        die("tcp client socket failed to connect to server '%s'", strerror(errno));

    return sock;
}

void tcpClientSocket_read(
    rtgn_tcpClientSocket_t sock,
    size_t bufferSize,
    void* buffer,
    ssize_t* bytesRead)
{
    *bytesRead = read(sock, buffer, bufferSize);
    if(bytesRead < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
        die("failed to read bytes from tcp client socket '%s'", strerror(errno));
}

void tcpClientSocket_write(
    rtgn_tcpClientSocket_t sock,
    size_t bufferSize,
    void* buffer)
{
    ssize_t bytesWritten = write(sock, buffer, bufferSize);
    if(bytesWritten < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
        die("write to tcp client socket was blocked");
    if(bytesWritten < 0)
        die("failed to tcp client socket");
    if(bytesWritten != bufferSize)
        die("failed to write full buffer to tcp client socket");
}

void tcpClientSocket_close(rtgn_tcpClientSocket_t sock)
{
    if(close(sock) < 0)
        die("failed to close tcp client socket '%s'", strerror(errno));
}
