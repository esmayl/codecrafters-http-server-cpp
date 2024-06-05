//
// Created by esmayl on 05/06/2024.
//

#ifndef GLOBALS_H
#define GLOBALS_H
#include <string>

class HttpPacket;

namespace Globals {
    extern const char* successResponse;
    extern const char* errorResponse;

    static std::string BuildResponse(const std::string& responseBody);
};
#endif //GLOBALS_H
