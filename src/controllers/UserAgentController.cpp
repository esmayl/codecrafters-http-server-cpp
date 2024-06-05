//
// Created by esmayl on 05/06/2024.
//

#include "UserAgentController.h"

#ifdef _WIN64
    void UserAgentController::SendResponse(SOCKET socket, HttpPacket &packet)
    {
        std::string tempString = Globals::BuildResponse(packet.GetUserAgent());

        std::cout << "Sending: " <<tempString<< std::endl;

        send(socket,tempString.c_str(),tempString.length(),0);
    }

#else

    void UserAgentController::SendResponse(int socket, HttpPacket &packet)
    {
        std::string responseString;

        responseString = Globals::BuildResponse(packet.GetUserAgent());

        std::cout << "Sending: " <<responseString<< std::endl;

        send(socket,responseString.c_str(),responseString.length(),0);
    }

#endif
