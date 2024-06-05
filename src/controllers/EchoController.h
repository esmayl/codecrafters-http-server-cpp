//
// Created by esmayl on 05/06/2024.
//

#ifndef ECHOCONTROLLER_H
#define ECHOCONTROLLER_H
#include "../HttpPacket.h"

#pragma once
#ifdef _WIN64

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <cstring>
#endif

class EchoController
{
    public:

        #ifdef _WIN64
            static void SendResponse(SOCKET socket, HttpPacket& packet);
        #else
            static void SendResponse(int socket,HttpPacket& packet);
        #endif

            static const char* BuildResponse(HttpPacket& packet);

};



#endif //ECHOCONTROLLER_H
