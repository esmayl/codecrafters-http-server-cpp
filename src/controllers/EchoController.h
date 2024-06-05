//
// Created by esmayl on 05/06/2024.
//

#ifndef ECHOCONTROLLER_H
#define ECHOCONTROLLER_H

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

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include "../HttpPacket.h"
#include "../Globals.h"

class EchoController
{
    public:

        #ifdef _WIN64
            static void SendResponse(SOCKET socket, HttpPacket& packet);
        #else
            static void SendResponse(int socket,HttpPacket& packet);
        #endif

            static std::string BuildResponse(HttpPacket& packet);

};



#endif //ECHOCONTROLLER_H
