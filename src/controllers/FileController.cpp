//
// Created by esmayl on 09/06/2024.
//

#include "FileController.h"

#include <fstream>

FileController::FileController(std::string fileFolder)
{
    this->fileFolder = std::move(fileFolder);
}

void FileController::GetResponse(HttpPacket* packet, const SocketWrapper* socketWrapper, const char* fileLocation) const
{
    std::string fileContent;
    std::string tempString;

    std::string filePath;

    filePath.append(fileFolder);
    filePath.append(fileLocation);

    std::ifstream inputFile(filePath);

    printf("Opening file: ");
    printf(filePath.c_str());
    printf("\n");

    if(!inputFile.is_open())
    {
        tempString = Globals::BuildResponse(packet,Globals::errorResponse, filePath+" file not found", CONTENTTYPE::PLAIN, false);
    }
    else
    {
        fileContent = std::string((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

        inputFile.close();

        tempString = Globals::BuildResponse(packet,Globals::getSuccessResponse, fileContent, CONTENTTYPE::OCTET, true);
    }

    send(socketWrapper->socket,tempString.c_str(),static_cast<int>(tempString.length()),0);
}

void FileController::PostResponse(HttpPacket* packet, const SocketWrapper* socketWrapper, const char* fileLocation, const char* dataToWrite, const std::streamsize* dataLength) const
{
    std::string filePath;

    filePath.append(fileFolder);
    filePath.append(fileLocation);

    std::ofstream outputFile(filePath);

    printf("Opening file: ");
    printf(filePath.c_str());
    printf("\n");
    printf("Writing: ");
    printf(dataToWrite);
    printf("\n");

    outputFile.write(dataToWrite,*dataLength);

    outputFile.close();

    std::string response = Globals::BuildResponse(packet,Globals::postSuccessResponse,"",CONTENTTYPE::OCTET,true);

    send(socketWrapper->socket,response.c_str(),static_cast<int>(response.length()),0);
}
