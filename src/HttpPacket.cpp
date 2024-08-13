//
// Created by esmayl on 05/06/2024.
//

#include "HttpPacket.h"

#include <iostream>
#include <vector>

HttpPacket::HttpPacket(std::string rawString)
{
    contentLength = 0;

    ssize_t foundIndex = 0;

    std::vector<std::string> splitString;

    size_t headerLength = 0;
    ssize_t startIndex = 0;

    while(foundIndex != -1)
    {
        foundIndex = rawString.find("\r\n",foundIndex);

        if(foundIndex == -1)
        {
            break;
        }
        std::string line = rawString.substr(startIndex,foundIndex-startIndex);

        if(line.find("Content-Length") != std::string::npos)
        {
            contentLength = std::stoll(line.substr(line.find(':')+1));
        }

        if(line.find("Accept-Encoding") != std::string::npos)
        {
            std::string encVal = line.substr(line.find(':')+1);
            contentEncoding = new char[encVal.length()];

            strcpy(contentEncoding,encVal.c_str());

            delete encVal;
        }

        splitString.push_back(line);

        startIndex = foundIndex;
        startIndex += 2;
        foundIndex++; // +1 to start searching further in the string

        headerLength+=line.size()+2;
    }

    if(splitString[0].find("GET") != -1)
    {
        requestType = HTTPMETHOD::GET;
    }
    if(splitString[0].find("POST") != -1)
    {
        requestType = HTTPMETHOD::POST;

        body = new char[contentLength+1];
        std::strcpy(body,rawString.substr(headerLength,contentLength).c_str());

        body += '\0';
    }
    if(splitString[0].find("PUT") != -1)
    {
        requestType = HTTPMETHOD::PUT;
    }
    if(splitString[0].find("DELETE") != -1)
    {
        requestType = HTTPMETHOD::DELETE;
    }


    size_t startEndpointChar = 0;
    size_t endEndPointChar = 0;

    startEndpointChar = splitString[0].find(' ',startEndpointChar);
    endEndPointChar = splitString[0].find(' ',startEndpointChar+1);

    startEndpointChar+=2; // +1 to skip the /
    endpoint = splitString[0].substr(startEndpointChar,endEndPointChar-startEndpointChar);


    for (size_t j=1;j < splitString.size();j++)
    {
        if(splitString[j].compare(0,10,"User-Agent") == 0)
        {
            size_t userAgentIndex = splitString[j].find(':');

            if(userAgentIndex != -1 && userAgentIndex+2 < splitString[j].length())
            {
                size_t crlf = splitString[j].find("\r\n",userAgentIndex);
                userAgent = splitString[j].substr(userAgentIndex+2,splitString[j].length()-crlf);
            }
        }
    }
}

HTTPMETHOD HttpPacket::GetRequestType()
{
    return requestType;
}

std::string& HttpPacket::GetEndpoint()
{
    return endpoint;
}

std::string & HttpPacket::GetUserAgent()
{
    return userAgent;
}

char* HttpPacket::GetBody()
{
    return body;
}

std::streamsize* HttpPacket::GetBodyLength()
{
    return &contentLength;
}

char* HttpPacket::GetContentEncoding()
{
    return contentEncoding;
}

