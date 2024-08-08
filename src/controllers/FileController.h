//
// Created by esmayl on 09/06/2024.
//

#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <utility>
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


class FileController
{

public:
    explicit FileController(std::string fileFolder);
#ifdef _WIN64
    static void SendResponse(SOCKET socket, char* directoryRoot, HttpPacket& packet);
#else
    static void SendResponse(int socket, char* directoryRoot, HttpPacket& packet);
#endif

private:
    std::string fileFolder;
    ~FileController();
};




#endif //FILECONTROLLER_H
