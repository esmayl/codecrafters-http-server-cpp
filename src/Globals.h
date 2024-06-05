//
// Created by esmayl on 05/06/2024.
//

#ifndef GLOBALS_H
#define GLOBALS_H
#include <string>

class HttpPacket;

class Globals {

public:
    static const char* successResponse;
    static const char* errorResponse;

    static std::string BuildResponse(const std::string& responseBody);
};
#endif //GLOBALS_H
