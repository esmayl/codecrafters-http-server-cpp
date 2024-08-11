//
// Created by esmayl on 05/06/2024.
//

#ifndef GLOBALS_H
#define GLOBALS_H
#include <string>

#include "CONTENTTYPE.h"

class HttpPacket;

class Globals {

public:
    static const char successResponse[];
    static const char errorResponse[];
    static const char contentType[];
    static const char contentLength[];

    static std::string BuildResponse(const std::string& responseBody, const CONTENTTYPE responseType, const bool succes);
};
#endif //GLOBALS_H
