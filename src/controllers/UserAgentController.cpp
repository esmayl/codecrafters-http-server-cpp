//
// Created by esmayl on 05/06/2024.
//

#include "UserAgentController.h"

void UserAgentController::GetResponse(HttpPacket* packet, SocketWrapper* connectedClient)
{
    std::string temp = Globals::BuildResponse(packet,Globals::getSuccessResponse, 0, CONTENTTYPE::PLAIN, true).ToString();
    send(connectedClient->socket,temp.c_str(),static_cast<int>(temp.length()),0);
}
