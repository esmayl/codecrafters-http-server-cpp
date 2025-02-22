//
// Created by esmayl on 05/06/2024.
//

#include "EchoController.h"

void EchoController::GetResponse(HttpPacket* packet, SocketWrapper* connectedClient)
{
    std::string endpoint = packet->GetEndpoint();
    size_t lastSlashIndex = endpoint.find_last_of('/');;
    lastSlashIndex++; // ++ to move past the /

    std::string contentBody = endpoint.substr(lastSlashIndex);
    std::string resp = Globals::BuildResponse(packet,Globals::getSuccessResponse, contentBody.length(), CONTENTTYPE::PLAIN, true).ToString();
    send(connectedClient->socket,resp.c_str(),static_cast<int>(resp.length()),0);
}