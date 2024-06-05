//
// Created by esmayl on 05/06/2024.
//

#include "EchoController.h"

#include <iostream>
#include <ostream>

#include "../Globals.h"

void EchoController::BuildResponse(HttpPacket& packet, const char* responseChar)
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
    buildResponse.append("\r\n");

    buildResponse.append(packetEndpoint.substr(lastSlashIndex));

    responseChar = buildResponse.c_str();

    std::cout << "Sending " <<responseChar<< std::endl;
}

#ifdef _WIN64
    void EchoController::SendResponse(SOCKET socket, HttpPacket& packet)
    {
        const char* responseChar;

        BuildResponse(packet, responseChar);

        send(socket,responseChar,strlen(responseChar),0);
    }

#else

    void EchoController::SendResponse(int socket,HttpPacket& packet)
    {
        const char* responseChar;

        BuildResponse(packet, responseChar);

        send(socket,responseChar,strlen(responseChar),0);
    }

#endif