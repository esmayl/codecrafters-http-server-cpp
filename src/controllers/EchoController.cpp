//
// Created by esmayl on 05/06/2024.
//

#include "EchoController.h"

void EchoController::SendResponse(const SocketWrapper* socketWrapper, HttpPacket* packet)
{
    std::string endpoint = packet->GetEndpoint();
    size_t lastSlashIndex = endpoint.find_last_of('/');;
    lastSlashIndex++; // ++ to move past the /

    std::string responseString = Globals::BuildResponse(packet,Globals::getSuccessResponse, endpoint.substr(lastSlashIndex), CONTENTTYPE::PLAIN, true);

    send(socketWrapper->socket,responseString.c_str(),static_cast<int>(responseString.size()),0);
}