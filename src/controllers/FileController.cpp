//
// Created by esmayl on 09/06/2024.
//

#include "FileController.h"

FileController::FileController(std::string fileFolder)
{
    this->fileFolder = std::move(fileFolder);
}

void FileController::SendResponse(SocketWrapper socketWrapper, char* directoryRoot, HttpPacket& packet)
{

}
