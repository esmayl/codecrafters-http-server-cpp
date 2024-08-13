//
// Created by esmayl on 05/06/2024.
//

#ifndef HTTPPACKET_H
#define HTTPPACKET_H

#pragma once
#include <string>
#include <cstring>  // For std::strcpy

#include "enums/HTTPMETHOD.h"

class HttpPacket
{
    public:
        explicit HttpPacket(std::string rawString);

        HTTPMETHOD GetRequestType();
        std::string& GetEndpoint();
        std::string& GetUserAgent();
        std::streamsize* GetBodyLength();
        char* GetContentEncoding();
        char* GetBody();

    private:
        HTTPMETHOD requestType;
        std::string endpoint;
        std::string userAgent;
        std::streamsize contentLength;
        const char* httpVersion;
        const char* host;
        char* body;
        char* contentEncoding;
};


#endif //HTTPPACKET_H
