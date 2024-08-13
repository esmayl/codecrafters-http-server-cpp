//
// Created by esmayl on 09/06/2024.
//

#include "FileController.h"

#include <fstream>

FileController::FileController(std::string fileFolder)
{
    this->fileFolder = std::move(fileFolder);
}

void FileController::GetResponse(HttpPacket* packet,SocketWrapper* socketWrapper, const char* fileLocation)
{
    std::string fileContent;
    std::string tempString;

    std::string filePath;

    filePath.append(fileFolder);
    filePath.append(fileLocation);

    std::ifstream inputFile(filePath);

    std::cout << "Opening file:" << filePath << std::endl;

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

    std::cout << "Sending: " << tempString << std::endl;

    send(socketWrapper->socket,tempString.c_str(),static_cast<int>(tempString.length()),0);
}

void FileController::PostResponse(HttpPacket* packet,SocketWrapper* socketWrapper, const char* fileLocation, char* dataToWrite, std::streamsize* dataLength)
{
    std::string filePath;

    filePath.append(fileFolder);
    filePath.append(fileLocation);

    std::ofstream outputFile(filePath);

    std::cout << "Opening file:" << filePath << std::endl;
    std::cout << "Writing: " << dataToWrite << std::endl;

    outputFile.write(dataToWrite,*dataLength);

    outputFile.close();

    std::string response = Globals::BuildResponse(packet,Globals::postSuccessResponse,"",CONTENTTYPE::OCTET,true);

    send(socketWrapper->socket,response.c_str(),static_cast<int>(response.length()),0);
}
