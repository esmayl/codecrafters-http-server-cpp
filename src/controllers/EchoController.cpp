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
    size_t contentLength;

    // First build the content
    char* content = Globals::BuildResponseBody(packet,contentBody.c_str(),contentLength);

    if (content == nullptr) {
        std::cerr << "Error: BuildResponseBody returned nullptr!" << std::endl;
    }

    // Created header using the accurate size of the content
    std::string resp = Globals::BuildResponse(packet,Globals::getSuccessResponse, contentLength, CONTENTTYPE::PLAIN, true).ToString();

    resp.append("\r\n");

    // Append the content to the response
    resp.append(std::string(content, contentLength)); // Ensures binary-safe append

    printf("Build response:\n\n");
    printf(resp.c_str());

    send(connectedClient->socket,resp.c_str(),static_cast<int>(resp.length()),0);
}