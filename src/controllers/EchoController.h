//
// Created by esmayl on 05/06/2024.
//

#ifndef ECHOCONTROLLER_H
#define ECHOCONTROLLER_H

#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sys/types.h>
#include "../Globals.h"
#include "../objects/HttpPacket.h"
#include "../objects/SocketWrapper.h"

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
        static void GetResponse(HttpPacket* packet, SocketWrapper* connectedClient);
};



#endif //ECHOCONTROLLER_H
