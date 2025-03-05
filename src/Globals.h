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
#include <stdint.h>
#include <zlib.h>

#include "enums/CONTENTTYPE.h"
#include "objects/HttpPacket.h"
#include "objects/HttpHeader.h"

class Globals
{

    public:
        static const char getSuccessResponse[];
        static const char postCreatedResponse[];
        static const char errorResponse[];
        static const char connectionClose[];
        static const char plainContentType[];
        static const char octetContentType[];
        static const std::string textFileExtensions[];
        static const std::vector<std::string> acceptedEncodings;

        static std::vector<uint8_t> GzipCompress(const std::vector<uint8_t>& inputData);

        static HttpHeader BuildResponse(HttpPacket* packet, const char* headerResponse, size_t contentLength, CONTENTTYPE responseType, bool succes);

        static std::vector<uint8_t> BuildResponseBody(HttpPacket* packet, const std::vector<uint8_t>& inputData,
                                                      size_t& outLength);
};
#endif //GLOBALS_H
