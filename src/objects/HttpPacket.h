//
// Created by esmayl on 05/06/2024.
//

#ifndef HTTPPACKET_H
#define HTTPPACKET_H

#pragma once
#include <string>
#include <cstring>  // For std::strcpy
#include <vector>
#include <iostream>
#include <vector>

#include "../enums/HTTPMETHOD.h"
#include "../utilities/StringUtils.h"

class HttpPacket
{
    public:
        explicit HttpPacket(const std::string& rawString);

        HTTPMETHOD GetRequestType();
        std::string& GetEndpoint();
        std::string& GetUserAgent();
        std::streamsize* GetBodyLength();
        std::vector<std::string>* GetContentEncoding();
        char* GetBody();

    private:
        HTTPMETHOD requestType;
        std::string endpoint;
        std::string userAgent;
        std::streamsize contentLength;
        const char* httpVersion;
        const char* host;
        char* body;
        std::vector<std::string> contentEncoding;
};


#endif //HTTPPACKET_H
