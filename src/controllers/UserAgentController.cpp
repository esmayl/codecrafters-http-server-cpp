//
// Created by esmayl on 05/06/2024.
//

#include "UserAgentController.h"

void UserAgentController::SendResponse(SocketWrapper sockerWrapper, HttpPacket &packet)
{
    std::string tempString = Globals::BuildResponse(packet.GetUserAgent(), CONTENTTYPE::PLAIN, true);

    std::cout << "Sending: " << tempString << std::endl;

    send(sockerWrapper.socket,tempString.c_str(),static_cast<int>(tempString.length()),0);
}
