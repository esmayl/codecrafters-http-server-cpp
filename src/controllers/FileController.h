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
    FileController(std::string fileFolder);
    void GetResponse(HttpPacket* packet, SocketWrapper* socketWrapper, const char* fileLocation);
    void PostResponse(HttpPacket* packet, SocketWrapper* socketWrapper, const char* fileLocation, char* dataToWrite, std::streamsize* dataLength);
private:
    std::string fileFolder;
};

#endif //FILECONTROLLER_H
