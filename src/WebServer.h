//
// Created by esmayl on 11/08/2024.
//

#ifndef WEBSERVER_H
#define WEBSERVER_H

#pragma once

#include <memory>
#include <string>
#include <utility>

#include "HttpPacket.h"
#include "SocketWrapper.h"
#include "controllers/FileController.h"
#include "controllers/EchoController.h"
#include "controllers/UserAgentController.h"

class WebServer
{
public:
    int port;

    explicit WebServer(int port);

    static HttpPacket ParseRequestHeader(const std::string &rawString);
    int Start();
    void AcceptConnection();
    void HandleRequest(SocketWrapper* connectedClient);
    void SetupDirectory(std::string folderRoot);

private:
    bool canUseFiles = false;
    SocketWrapper serverSocketWrapper;
    std::unique_ptr<FileController> fileControllerInstance;
};


#endif //WEBSERVER_H
