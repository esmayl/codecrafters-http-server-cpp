//
// Created by esmayl on 05/06/2024.
//

#ifndef GLOBALS_H
#define GLOBALS_H
#include <string>

#include "enums/CONTENTTYPE.h"

class HttpPacket;

class Globals
{

    public:
        static const char getSuccessResponse[];
        static const char postSuccessResponse[];
        static const char errorResponse[];
        static const char contentType[];
        static const char contentLength[];
        static const char connectionClose[];
        static const char contentEncoding[];

        static std::string Globals::BuildResponse(HttpPacket* packet,const char* headerResponse, const std::string &responseBody, const CONTENTTYPE responseType, const bool succes);
};
#endif //GLOBALS_H
