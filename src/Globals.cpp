//
// Created by esmayl on 05/06/2024.
//

#include "Globals.h"

const char* Globals::successResponse = "HTTP/1.1 200 OK\r\n";
const char* Globals::errorResponse = "HTTP/1.1 404 Not Found\r\n";

std::string Globals::BuildResponse(const std::string &responseBody)
{
    std::string buildResponse;
    buildResponse.append(successResponse);
    buildResponse.append("Content-Type: text/plain\r\nContent-Length:");

    buildResponse.append(std::to_string(responseBody.length()));
    buildResponse.append("\r\n\r\n");

    buildResponse.append(responseBody);

    return buildResponse;
}

