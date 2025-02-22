//
// Created by esmayl on 18/02/2025.
//

#ifndef HTTPHEADER_H
#define HTTPHEADER_H

#pragma once
#include <string>
#include <sstream>

class HttpHeader {

public:
    std::string responseStatus;
    std::string contentEncoding;
    std::string contentType;
    std::string contentLength;
    std::string transferEncoding;
    bool chunked;
    bool success;

    std::string ToString();
};



#endif //HTTPHEADER_H
