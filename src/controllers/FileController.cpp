//
// Created by esmayl on 09/06/2024.
//

#include "FileController.h"

FileController::FileController(std::string fileFolder)
{
    this->fileFolder = std::move(fileFolder);
}

void FileController::GetResponse(HttpPacket* packet, SocketWrapper* connectedClient) const
{
    std::string fileContent;
    std::string tempString;

    std::string filePath;

    filePath.append(fileFolder);
    filePath.append(packet->GetEndpoint().substr(7));

    std::ifstream inputFile(filePath,std::ios::binary|std::ios::in);

    printf("Opening file: ");
    printf(filePath.c_str());
    printf("\n");

    if(!inputFile.is_open())
    {
        tempString = Globals::BuildResponse(packet,Globals::errorResponse,0 , CONTENTTYPE::PLAIN, false).ToString();
        send(connectedClient->socket,tempString.c_str(),static_cast<int>(tempString.length()),0);
    }
    else
    {
        //fileContent = std::string((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

        int fileSize = std::filesystem::file_size(filePath);
        char* buffer = new char[1024];

        while (inputFile.read(buffer,1024))
        {
            tempString = Globals::BuildResponse(packet,Globals::getSuccessResponse, fileSize, CONTENTTYPE::OCTET, true).ToString();
            send(connectedClient->socket,tempString.c_str(),static_cast<int>(tempString.length()),0);
        }

        inputFile.close();

        delete[] buffer;
    }

}

void FileController::PostResponse(HttpPacket* packet, SocketWrapper* connectedClient) const
{
    std::string filePath;

    filePath.append(fileFolder);
    filePath.append(packet->GetEndpoint().substr(7));

    printf("Opening file: ");
    printf(filePath.c_str());
    printf("\n");
    printf("Writing: ");
    printf(packet->GetBody());
    printf("\n");

    if(std::filesystem::exists(filePath))
    {
        std::ofstream outputFile(filePath);

        // Move to the beginning of the file
        outputFile.seekp(0, std::ios::beg);
        outputFile.write(packet->GetBody(),*packet->GetBodyLength());
        outputFile.close();

        std::string temp = Globals::BuildResponse(packet,Globals::getSuccessResponse,0,CONTENTTYPE::OCTET, true).ToString();
        send(connectedClient->socket,temp.c_str(),static_cast<int>(temp.length()),0);

    }

    std::ofstream outputFile(filePath);

    outputFile.write(packet->GetBody(),*packet->GetBodyLength());
    outputFile.close();

    std::string temp = Globals::BuildResponse(packet,Globals::postCreatedResponse,0,CONTENTTYPE::OCTET, true).ToString();

    send(connectedClient->socket,temp.c_str(),static_cast<int>(temp.length()),0);
}
