//
// Created by esmayl on 05/06/2024.
//

#include "Globals.h"
namespace Globals {
    const char* successResponse = "HTTP/1.1 200 OK\r\n";
    const char* errorResponse = "HTTP/1.1 404 Not Found\r\n";

    std::string BuildResponse(const std::string &responseBody)
    {
        std::string buildResponse;
        buildResponse.append(Globals::successResponse);
        buildResponse.append("Content-Type: text/plain\r\nContent-Length:");

        size_t lastSlashIndex = responseBody.find_last_of('/');;
        lastSlashIndex++; // ++ to move past the /
        size_t contentLength = responseBody.length() - lastSlashIndex;

        // buildResponse.append(std::string_literals::);
        buildResponse.append(std::to_string(contentLength));
        buildResponse.append("\r\n\r\n");

        buildResponse.append(responseBody.substr(lastSlashIndex));

        return buildResponse;
    }
}
