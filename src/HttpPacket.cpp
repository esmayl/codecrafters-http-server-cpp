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

    int headerLength = 0;
    int startIndex = 0;

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
            contentLength = std::atoi(line.substr(line.find(':')+1).c_str());
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

        body = rawString.substr(headerLength+4,contentLength).c_str(); // +4 to skip the /r/n/r/n

        std::cout<< "Body: "<< body << std::endl;
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

