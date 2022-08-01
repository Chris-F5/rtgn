#ifndef RTGN_NETWORK_ADDRESS_H
#define RTGN_NETWORK_ADDRESS_H

#include "./common_types.h"

int rtgn_networkAddress(
    const char* addrString,
    int port,
    rtgn_networkAddress_t* addr);

#endif
