//
// Created by esmayl on 05/06/2024.
//

#include "EchoController.h"

#ifdef _WIN64
    void EchoController::SendResponse(SOCKET socket, HttpPacket& packet)
    {
        const char* responseChar;

        responseChar = Globals::BuildResponse(packet.GetEndpoint()).c_str();

        std::cout << "Sending: " <<responseChar<< std::endl;

        send(socket,responseChar,strlen(responseChar),0);
    }

#else

    void EchoController::SendResponse(int socket,HttpPacket& packet)
    {
        const char* responseChar;

        std::string test = Globals::BuildResponse(packet.GetEndpoint());

        std::cout << "Sending " << test.c_str() << std::endl;

        send(socket,test.c_str(),test.size(),0);
    }

#endif