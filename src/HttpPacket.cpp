//
// Created by esmayl on 05/06/2024.
//

#include "HttpPacket.h"

#include <vector>

HttpPacket::HttpPacket(std::string rawString)
{
    ssize_t foundIndex = 0;

    std::vector<std::string> splitString;

    int startIndex = 0;
    while(foundIndex != -1)
    {
        foundIndex = rawString.find("\r\n",foundIndex);

        if(foundIndex == -1)
        {
            break;
        }

        splitString.push_back(rawString.substr(startIndex,foundIndex-startIndex));

        startIndex = foundIndex;
        startIndex += 2;
        foundIndex++; // +1 to start searching further in the string
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


    for (size_t j=1;j < splitString.size();j++)
    {
        if(splitString[j].compare(0,10,"User-Agent") == 0)
        {
            int userAgentIndex = splitString[j].find(':');

            if(userAgentIndex != -1 && userAgentIndex+2 < splitString[j].length())
            {
                int crlf = splitString[j].find("\r\n",userAgentIndex);
                userAgent = splitString[j].substr(userAgentIndex+2,splitString[j].length()-crlf);
                break;
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

