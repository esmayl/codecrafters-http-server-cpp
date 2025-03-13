//
// Created by esmayl on 18/02/2025.
//

#include "HttpHeader.h"

std::string HttpHeader::ToString()
{
    std::stringstream stringStream;

    stringStream << "HTTP/1.1 " << this->responseStatus << "\r\n";

    if (!this->success)
    {
        return stringStream.str();
    }

    stringStream << "Content-Encoding: " << this->contentEncoding << "\r\n";
    stringStream << "Content-Type: " << this->contentType << "\r\n";

    stringStream << "Content-Length: " << this->contentLength << "\r\n";
    if (!this->chunked)
    {
    }
    else
    {
        stringStream << "Transfer-Encoding: chunked";

        if (this->contentEncoding == "gzip")
        {
            stringStream << ",gzip";
        }

        stringStream << "\r\n";
    }


    return stringStream.str();
}
