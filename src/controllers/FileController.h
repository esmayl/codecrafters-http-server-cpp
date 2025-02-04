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
#include "../objects/HttpPacket.h"
#include "../Globals.h"
#include "../objects/SocketWrapper.h"

class FileController
{

public:
    FileController(std::string fileFolder);
    void GetResponse(HttpPacket* packet, const SocketWrapper* socketWrapper, const char* fileLocation) const;
    void PostResponse(HttpPacket* packet, const SocketWrapper* socketWrapper, const char* fileLocation, const char* dataToWrite, const std::streamsize* dataLength) const;
private:
    std::string fileFolder;
};

#endif //FILECONTROLLER_H
