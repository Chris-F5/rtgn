#include "rtgn/network_address.h"

#include <arpa/inet.h>
#include <string.h>

int rtgn_networkAddress(
    const char* addrString,
    int port,
    rtgn_networkAddress_t* addr)
{
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    if(inet_aton(addrString, &addr->sin_addr) == 0)
        return -1;
    bzero(addr->sin_zero, sizeof(addr->sin_zero));
    return 0;
}
