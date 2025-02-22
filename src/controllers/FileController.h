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
#include <sys/types.h>
#include <utility>
#include <fstream>
#include <filesystem>

#include "../Globals.h"
#include "../objects/HttpPacket.h"
#include "../objects/SocketWrapper.h"

class FileController
{

public:
    explicit FileController(std::string fileFolder);

    void GetResponse(HttpPacket* packet, SocketWrapper* connectedClient) const;

    void PostResponse(HttpPacket* packet, SocketWrapper* connectedClient) const;
private:
    std::string fileFolder;
};

#endif //FILECONTROLLER_H
