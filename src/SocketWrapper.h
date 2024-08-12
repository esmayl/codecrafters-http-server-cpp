//
// Created by esmayl on 11/08/2024.
//

#ifndef SOCKETWRAPPER_H
#define SOCKETWRAPPER_H

#pragma once

#ifdef _WIN64
    #include <winsock2.h>
    #include <Ws2tcpip.h>
    #include <cstdio>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <cstring>
#endif


class SocketWrapper
{
    public:
    #ifdef _WIN64
        SOCKET socket;
        char reuse;
    #else
        int socket;
        int reuse;
    #endif

};



#endif //SOCKETWRAPPER_H
