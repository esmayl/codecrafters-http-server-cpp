//
// Created by esmayl on 05/06/2024.
//

#include "Globals.h"

const char Globals::successResponse[] = "HTTP/1.1 200 OK\r\n";
const char Globals::errorResponse[] = "HTTP/1.1 404 Not Found\r\n";
const char Globals::contentType[] = "Content-Type: ";
const char Globals::contentLength[] = "Content-Length: ";

std::string Globals::BuildResponse(const std::string &responseBody, const CONTENTTYPE responseType, const bool succes)
{
    std::string buildResponse;

    if(succes)
    {
        buildResponse.append(successResponse);
    }
    else
    {
        buildResponse.append(errorResponse);
        buildResponse.append("\r\n");

        return buildResponse;
    }

    buildResponse.append(contentType);

    switch (responseType)
    {
        case CONTENTTYPE::PLAIN:
            buildResponse.append("text/plain\r\n");
            break;
        case CONTENTTYPE::OCTET:
            buildResponse.append("application/octet-stream\r\n");
            break;
    }

    buildResponse.append(contentLength);

    buildResponse.append(std::to_string(responseBody.length()));
    buildResponse.append("\r\n\r\n");

    if(!responseBody.empty())
    {
        buildResponse.append(responseBody);
    }
    else
    {
        buildResponse.append("\r\n");
    }

    return buildResponse;
}

