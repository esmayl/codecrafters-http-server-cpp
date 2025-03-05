//
// Created by esmayl on 09/06/2024.
//

#include "FileController.h"

FileController::FileController(std::string fileFolder)
{
    this->fileFolder = std::move(fileFolder);
}

/// Gets file from disk using the filename in the endpoint, must include file extension
/// File extension is used for setting file open mode
/// @param packet request packet
/// @param connectedClient socket wrapped in a class to facilitatie linux and windows
void FileController::GetResponse(HttpPacket* packet, SocketWrapper* connectedClient) const
{
    std::string responseHeader;

    std::string filePath;
    std::string fileName;

    fileName = packet->GetEndpoint().substr(7);

    filePath.append(this->fileFolder);
    filePath.append(fileName);

    std::string fileExtension = filePath.substr(filePath.find_last_of('.'));
    bool isText = false;

    for ( size_t i=0;i<Globals::textFileExtensions->length();i++ )
    {
        if (Globals::textFileExtensions[i] == fileExtension)
        {
            isText = true;
        }
    }

    std::ios::openmode mode = std::ios::in;
    if (!isText)
    {
        mode |= std::ios::binary;
    }

    std::ifstream inputFile(filePath,mode);

    printf("Opening file: ");
    printf(filePath.c_str());
    printf("\n");

    if(!inputFile.is_open())
    {
        responseHeader = Globals::BuildResponse(packet,Globals::errorResponse,0 , CONTENTTYPE::PLAIN, false).ToString();
        send(connectedClient->socket,responseHeader.c_str(),static_cast<int>(responseHeader.length()),0);
    }
    else
    {
        size_t fileSize = std::filesystem::file_size(filePath);
        CONTENTTYPE responseType = CONTENTTYPE::PLAIN;

        if (mode & std::ios::binary)
        {
            responseType = CONTENTTYPE::OCTET;
        }


        if (fileSize < 1024)
        {
            std::vector<uint8_t> fileData(fileSize);
            fileData = std::vector<uint8_t>((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

            // std::string fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
            // inputFile.read(reinterpret_cast<char*>(fileData.data()), fileSize); // Read file into buffer

            std::vector<uint8_t> content = Globals::BuildResponseBody(packet,fileData,fileSize);

            responseHeader = Globals::BuildResponse(packet,Globals::getSuccessResponse, fileSize, responseType, true).ToString();

            responseHeader.append("\r\n");
            send(connectedClient->socket,responseHeader.c_str(),static_cast<int>(responseHeader.length()),0);
            send(connectedClient->socket,reinterpret_cast<const char*>(content.data()),static_cast<int>(content.size()),0);

            printf("Build response:\n\n");
            printf(responseHeader.c_str());

            return;
        }

        responseHeader = Globals::BuildResponse(packet,Globals::getSuccessResponse, fileSize, responseType, true).ToString();
        responseHeader.append("\r\n");
        send(connectedClient->socket,responseHeader.c_str(),static_cast<int>(responseHeader.length()),0);

        std::streamsize bufferSize = 1024 * 1024;
        std::vector<uint8_t> fileData(bufferSize);
        std::vector<uint8_t> content(bufferSize);

        char chunkHeader[10];
        GzipStream gzipStream;

        int errorCode = 0;

        while (inputFile)
        {
            inputFile.read(reinterpret_cast<char*>(fileData.data()),bufferSize);

            size_t readBytes = inputFile.gcount(); // Get actual bytes read
            if (readBytes == 0) break; // No more data

            // std::vector<uint8_t> content = Globals::BuildResponseBody(packet,fileData,readBytes);

            // if (packet->GetContentEncoding() == "gzip")
            // {
            //
            // }

            // content = gzipStream.compress(std::vector<uint8_t>(fileData.begin(),fileData.end()),false);

            // Build a chunk
            sprintf(chunkHeader, "%zx\r\n", content.size());

            errorCode = send(connectedClient->socket, chunkHeader, static_cast<int>(strlen(chunkHeader)), 0);
            if (errorCode == -1 )
            {
                break;
            }

            errorCode = send(connectedClient->socket, reinterpret_cast<const char*>(fileData.data()), static_cast<int>(fileData.size()), 0);
            if (errorCode == -1 )
            {
                break;
            }

            errorCode = send(connectedClient->socket, "\r\n", 2, 0);
            if (errorCode == -1 )
            {
                break;
                // Handle error: log the issue and possibly close the connection
            }
            printf("Sending chunk\r\n");
        }

        // std::vector<uint8_t> finalFlush = gzipStream.compress({}, true);
        //
        // if (!finalFlush.empty()) {
        //     sprintf(chunkHeader, "%zx\r\n", finalFlush.size());
        //     send(connectedClient->socket, chunkHeader, static_cast<int>(strlen(chunkHeader)), 0);
        //     send(connectedClient->socket, reinterpret_cast<const char*>(finalFlush.data()), static_cast<int>(finalFlush.size()), 0);
        //     send(connectedClient->socket, "\r\n", 2, 0);
        // }

        inputFile.close();

        if (errorCode > -1 )
        {
            send(connectedClient->socket, "0\r\n\r\n", 5, 0);
        }

#ifdef _WIN64
        closesocket(connectedClient->socket);
#else
        close(connectedClient->socket);
#endif

        return;
        //
        // char* response = Globals::BuildResponseBody(packet,buffer,)
        // while (inputFile.read(buffer,1024))
        // {
        //
        //     send(connectedClient->socket,responseHeader.c_str(),static_cast<int>(responseHeader.length()),0);
        // }
        //
        // inputFile.close();
        //
        // delete[] buffer;
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
