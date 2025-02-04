//
// Created by esmayl on 05/06/2024.
//

#include "UserAgentController.h"

void UserAgentController::SendResponse(const SocketWrapper* socketWrapper, HttpPacket* packet)
{
    std::string tempString = Globals::BuildResponse(packet,Globals::getSuccessResponse, packet->GetUserAgent(), CONTENTTYPE::PLAIN, true);

    send(socketWrapper->socket,tempString.c_str(),static_cast<int>(tempString.length()),0);
}
