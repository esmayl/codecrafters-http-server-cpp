//
// Created by esmayl on 05/06/2024.
//

#include "EchoController.h"

void EchoController::GetResponse(HttpPacket* packet, SocketWrapper* connectedClient)
{
    size_t contentLength;
    std::string contentBody = packet->GetEndpoint().substr(packet->GetEndpoint().find_last_of('/')+1);

    std::vector<uint8_t> echoData(contentBody.begin(), contentBody.end());

    echoData = Globals::BuildResponseBody(packet,echoData,contentLength);

    std::string resp = Globals::BuildResponse(packet,Globals::getSuccessResponse, contentLength, CONTENTTYPE::PLAIN, true).ToString();
    resp.append("\r\n");

    send(connectedClient->socket,resp.c_str(),static_cast<int>(resp.length()),0);

    send(connectedClient->socket,reinterpret_cast<const char*>(echoData.data()),static_cast<int>(echoData.size()),0);
}