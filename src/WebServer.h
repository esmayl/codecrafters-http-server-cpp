//
// Created by esmayl on 11/08/2024.
//

#ifndef WEBSERVER_H
#define WEBSERVER_H

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <memory>
#include <thread>
#include <filesystem>

#include "objects/HttpPacket.h"
#include "objects/SocketWrapper.h"
#include "controllers/FileController.h"
#include "controllers/EchoController.h"
#include "controllers/UserAgentController.h"
#include "utilities/ThreadPool.h"
#include "enums/HTTPMETHOD.h"

class WebServer
{
public:
    int port;

    explicit WebServer(int port);

    static HttpPacket ParseRequestHeader(const std::string &rawString);
    int Start();
    void AcceptConnection();
    void HandleRequest(SocketWrapper* connectedClient);

    bool SetupDirectory(std::string folderRoot);

private:
    bool canUseFiles = false;
    SocketWrapper serverSocketWrapper;
    std::unique_ptr<FileController> fileControllerInstance;
    ThreadPool threadPool;
};


#endif //WEBSERVER_H
