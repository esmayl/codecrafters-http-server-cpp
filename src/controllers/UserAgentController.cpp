//
// Created by esmayl on 05/06/2024.
//

#include "UserAgentController.h"

void UserAgentController::GetResponse(HttpPacket* packet, SocketWrapper* connectedClient)
{
    size_t contentLength;
    char* content = Globals::BuildResponseBody(packet,packet->GetUserAgent().c_str(),contentLength);

    std::string resp = Globals::BuildResponse(packet,Globals::getSuccessResponse, contentLength, CONTENTTYPE::PLAIN, true).ToString();

    resp.append("\r\n");
    resp.append(std::string(content,contentLength));

    send(connectedClient->socket,resp.c_str(),static_cast<int>(resp.length()),0);
}
