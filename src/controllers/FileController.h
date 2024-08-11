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
#include "../SocketWrapper.h"

class FileController
{

public:
    explicit FileController(std::string fileFolder);
    static void SendResponse(SocketWrapper socketWrapper, char* directoryRoot, HttpPacket& packet);

private:
    std::string fileFolder;
};

#endif //FILECONTROLLER_H
