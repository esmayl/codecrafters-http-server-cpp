//
// Created by esmayl on 05/06/2024.
//

#ifndef GLOBALS_H
#define GLOBALS_H

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <ostream>

#include "HttpPacket.h"
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
        static const std::vector<std::string> acceptedEncodings;

        static std::string BuildResponse(HttpPacket* packet,const char* headerResponse, const std::string &responseBody, const CONTENTTYPE responseType, const bool succes);
};
#endif //GLOBALS_H
