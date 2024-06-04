//
// Created by esmayl on 05/06/2024.
//

#include "HttpPacket.h"

HttpPacket::HttpPacket(std::string rawString)
{
    ssize_t foundIndex = 0;

    std::string splitString[5];

    int i = 0;
    int startIndex = 0;
    while(foundIndex != -1)
    {
        foundIndex = rawString.find("\r\n",foundIndex);

        if(foundIndex == -1)
        {
            break;
        }

        splitString[i] = rawString.substr(startIndex,foundIndex-startIndex);

        startIndex = foundIndex;
        startIndex += 2;
        foundIndex++; // +1 to start searching further in the string

        if(i < 4)
        {
            i++;
        }
        else
        {
            break;
        }
    }

    if(splitString[0].find("GET") != -1)
    {
        requestType = HTTPMETHOD::GET;
    }
    if(splitString[0].find("POST") != -1)
    {
        requestType = HTTPMETHOD::POST;
    }
    if(splitString[0].find("PUT") != -1)
    {
        requestType = HTTPMETHOD::PUT;
    }
    if(splitString[0].find("DELETE") != -1)
    {
        requestType = HTTPMETHOD::DELETE;
    }


    int startEndpointChar = 0;
    int endEndPointChar = 0;

    startEndpointChar = splitString[0].find(' ',startEndpointChar);
    endEndPointChar = splitString[0].find(' ',startEndpointChar+1);

    startEndpointChar+=2; // +1 to skip the /
    endpoint = splitString[0].substr(startEndpointChar,endEndPointChar-startEndpointChar);
}

HTTPMETHOD HttpPacket::GetRequestType()
{
    return requestType;
}

std::string& HttpPacket::GetEndpoint()
{
    return endpoint;
}

