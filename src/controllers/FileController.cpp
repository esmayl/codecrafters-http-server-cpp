//
// Created by esmayl on 09/06/2024.
//

#include "FileController.h"

#include <fstream>

FileController::FileController(std::string fileFolder)
{
    this->fileFolder = std::move(fileFolder);
}

void FileController::SendResponse(SocketWrapper* socketWrapper, const char* fileLocation)
{
    std::string fileContent;
    std::string tempString;

    std::string filePath;

    filePath.append(fileFolder);
    filePath.append(fileLocation);

    std::ifstream inputFile(filePath);

    if(!inputFile.is_open())
    {
        tempString = Globals::BuildResponse(filePath+" file not found", CONTENTTYPE::PLAIN, false);
    }
    else
    {
        fileContent = std::string((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

        inputFile.close();

        tempString = Globals::BuildResponse(fileContent, CONTENTTYPE::OCTET, true);
    }

    std::cout << "Sending: " << tempString << std::endl;

    send(socketWrapper->socket,tempString.c_str(),static_cast<int>(tempString.length()),0);
}
