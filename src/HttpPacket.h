//
// Created by esmayl on 05/06/2024.
//

#ifndef HTTPPACKET_H
#define HTTPPACKET_H

#pragma once
#include <string>

#include "enums/HTTPMETHOD.h"

class HttpPacket
{
    public:
        explicit HttpPacket(std::string rawString);
        HTTPMETHOD GetRequestType();
        std::string& GetEndpoint();
        std::string& GetUserAgent();
        const char* GetBody();
        std::streamsize GetBodyLength();
    private:
        HTTPMETHOD requestType;
        std::string endpoint;
        const char* httpVersion;
        std::string userAgent;
        const char* host;
        std::streamsize contentLength;
        const char* body;
};


#endif //HTTPPACKET_H
