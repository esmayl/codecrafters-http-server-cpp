//
// Created by esmayl on 05/06/2024.
//

#include "EchoController.h"

void EchoController::SendResponse(SocketWrapper socketWrapper, HttpPacket& packet)
{
    std::string endpoint = packet.GetEndpoint();
    size_t lastSlashIndex = endpoint.find_last_of('/');;
    lastSlashIndex++; // ++ to move past the /

    endpoint = Globals::BuildResponse(endpoint.substr(lastSlashIndex), CONTENTTYPE::PLAIN);

    std::cout << "Sending: " <<endpoint<< std::endl;

    send(socketWrapper.socket,endpoint.c_str(),static_cast<int>(endpoint.size()),0);
}