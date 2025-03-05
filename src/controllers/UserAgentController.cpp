//
// Created by esmayl on 05/06/2024.
//

#include "UserAgentController.h"

void UserAgentController::GetResponse(HttpPacket* packet, SocketWrapper* connectedClient)
{
    size_t contentLength;
    std::vector<uint8_t> userAgentData(packet->GetUserAgent().begin(),packet->GetUserAgent().end());

    userAgentData = Globals::BuildResponseBody(packet,userAgentData,contentLength);

    std::string resp = Globals::BuildResponse(packet,Globals::getSuccessResponse, contentLength, CONTENTTYPE::PLAIN, true).ToString();
    resp.append("\r\n");

    send(connectedClient->socket,resp.c_str(),static_cast<int>(resp.length()),0);

    send(connectedClient->socket,reinterpret_cast<const char*>(userAgentData.data()),static_cast<int>(userAgentData.size()),0);

}
