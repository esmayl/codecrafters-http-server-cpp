//
// Created by esmayl on 09/06/2024.
//

#include "FileController.h"

FileController::FileController(std::string fileFolder)
{
    this->fileFolder = std::move(fileFolder);
}


#ifdef _WIN64
    void FileController::SendResponse(SOCKET socket, char* directoryRoot, HttpPacket& packet)
    {

    }
#else
    void FileController::SendResponse(int socket, char* directoryRoot, HttpPacket& packet)
    {

    }
#endif