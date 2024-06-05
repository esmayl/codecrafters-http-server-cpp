//
// Created by esmayl on 05/06/2024.
//

#ifndef USERAGENTCONTROLLER_H
#define USERAGENTCONTROLLER_H

#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>

#include "../HttpPacket.h"
#include "../Globals.h"

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

class UserAgentController
{
    public:
        #ifdef _WIN64
            static void SendResponse(SOCKET socket, HttpPacket& packet);
        #else
            static void SendResponse(int socket,HttpPacket& packet);
        #endif
};



#endif //USERAGENTCONTROLLER_H
