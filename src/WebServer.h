//
// Created by esmayl on 11/08/2024.
//

#ifndef WEBSERVER_H
#define WEBSERVER_H

#pragma once

#include <string>

#include "HttpPacket.h"
#include "SocketWrapper.h"
#include "controllers/FileController.h"
#include "controllers/EchoController.h"
#include "controllers/UserAgentController.h"

class WebServer
{
public:
    int port;

    WebServer(int port, int receiveBufferSize);

    static HttpPacket ParseRequestHeader(const std::string &rawString);
    static SocketWrapper AcceptConnection(SocketWrapper connectedClient);
    void HandleRequest(SocketWrapper connectedClient);
    void SetupDirectory(std::string folderRoot);

private:
    char* receiveBuffer;
    int receiveBufferSize;
    bool canUseFiles = false;
    FileController* fileControllerInstance = nullptr;
};


#endif //WEBSERVER_H
