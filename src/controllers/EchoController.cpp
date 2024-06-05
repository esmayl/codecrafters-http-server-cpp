//
// Created by esmayl on 05/06/2024.
//

#include "EchoController.h"

#include <iostream>
#include <ostream>

#include "../Globals.h"

const char* EchoController::BuildResponse(HttpPacket& packet)
{
    std::string buildResponse;
    buildResponse.append(Globals::successResponse);
    buildResponse.append("Content-Type: text/plain\r\nContent-Length:");

    std::string packetEndpoint = packet.GetEndpoint();

    size_t lastSlashIndex = packetEndpoint.find_last_of('/');;
    lastSlashIndex++; // ++ to move past the /
    size_t contentLength = packetEndpoint.length() - lastSlashIndex;

    buildResponse.append(" ");
    buildResponse.append(std::to_string(contentLength));
    buildResponse.append("\r\n\r\n");

    buildResponse.append(packetEndpoint.substr(lastSlashIndex));

    return buildResponse.c_str();
}

#ifdef _WIN64
    void EchoController::SendResponse(SOCKET socket, HttpPacket& packet)
    {
        const char* responseChar;

        responseChar = BuildResponse(packet);

        std::cout << "Sending " <<responseChar<< std::endl;

        send(socket,responseChar,strlen(responseChar),0);
    }

#else

    void EchoController::SendResponse(int socket,HttpPacket& packet)
    {
        const char* responseChar;

        responseChar = BuildResponse(packet);

        std::cout << "Sending " <<responseChar<< std::endl;

        send(socket,responseChar,strlen(responseChar),0);
    }

#endif