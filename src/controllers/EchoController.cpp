//
// Created by esmayl on 05/06/2024.
//

#include "EchoController.h"

#ifdef _WIN64
    void EchoController::SendResponse(SOCKET socket, HttpPacket& packet)
    {
        const char* responseChar;

        std::string endpoint = packet.GetEndpoint();
        size_t lastSlashIndex = endpoint.find_last_of('/');;
        lastSlashIndex++; // ++ to move past the /

        endpoint = Globals::BuildResponse(endpoint.substr(lastSlashIndex), CONTENTTYPE::PLAIN);

        std::cout << "Sending: " <<endpoint<< std::endl;

        send(socket,endpoint.c_str(),static_cast<int>(endpoint.size()),0);
    }

#else

    void EchoController::SendResponse(int socket,HttpPacket& packet)
    {
        const char* responseChar;

        std::string endpoint = packet.GetEndpoint();
        size_t lastSlashIndex = endpoint.find_last_of('/');;
        lastSlashIndex++; // ++ to move past the /

        endpoint = Globals::BuildResponse(endpoint.substr(lastSlashIndex), CONTENTTYPE::PLAIN);

        std::cout << "Sending " << endpoint.c_str() << std::endl;

        send(socket,endpoint.c_str(),static_cast<int>(endpoint.size()),0);
    }

#endif