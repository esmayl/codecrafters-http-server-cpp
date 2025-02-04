//
// Created by esmayl on 05/06/2024.
//

#include "Globals.h"

const char Globals::getSuccessResponse[] = "HTTP/1.1 200 OK\r\n";
const char Globals::postSuccessResponse[] = "HTTP/1.1 201 Created\r\n";
const char Globals::errorResponse[] = "HTTP/1.1 404 Not Found\r\n";
const char Globals::contentType[] = "Content-Type: ";
const char Globals::contentLength[] = "Content-Length: ";
const char Globals::contentEncoding[] = "Content-Encoding: ";
const std::vector<std::string> Globals::acceptedEncodings = {"gzip", "deflate", "br"};

std::string Globals::BuildResponse(HttpPacket* packet,const char* headerResponse, std::string responseBody, const CONTENTTYPE responseType, const bool succes)
{
    std::string buildResponse;
    bool isAccepted = false;
    int acceptedIndex = -1;

    if(succes)
    {
        buildResponse.append(headerResponse);

    }
    else
    {
        buildResponse.append(headerResponse);
        buildResponse.append("\r\n");

        return buildResponse;
    }

    if(!packet->GetContentEncoding()->empty())
    {

        std::vector<std::string>* requestEncoding = packet->GetContentEncoding();

        for(const auto& str: acceptedEncodings)
        {
            acceptedIndex++;
            size_t requestSize = requestEncoding->size();

            for(int i=0;i<requestSize;i++)
            {
                if(str == requestEncoding->at(i))
                {
                    isAccepted = true;
                    break;
                }
            }

            if(isAccepted)
            {
                break;
            }
        }


        if(isAccepted)
        {
            buildResponse.append(Globals::contentEncoding);
            buildResponse.append(acceptedEncodings[acceptedIndex]);
            std::cout << "Building response with content encoding: " << acceptedEncodings[acceptedIndex] << std::endl;
            buildResponse.append("\r\n");

            if(acceptedEncodings[acceptedIndex] == "gzip")
            {
                responseBody = GzipCompress(responseBody);
            }
        }
    }

    buildResponse.append(contentType);

    switch (responseType)
    {
        case CONTENTTYPE::PLAIN:
            buildResponse.append("text/plain\r\n");
            break;
        case CONTENTTYPE::OCTET:
            buildResponse.append("application/octet-stream\r\n");
            break;
    }

    buildResponse.append(contentLength);

    buildResponse.append(std::to_string(responseBody.length()));
    buildResponse.append("\r\n\r\n");

    std::cout << "Build response:\n\n" << buildResponse;

    if(!responseBody.empty())
    {
        buildResponse.append(responseBody);
        std::cout << "[Gzip compressed body, content length: " << std::to_string(responseBody.length()) << "]" << "\n\n" << std::endl;
    }
    else
    {
        buildResponse.append("\r\n");
        std::cout << "\n\n" << std::endl;
    }

    return buildResponse;
}


std::string Globals::GzipCompress(std::string inputString)
{
    z_stream deflateStream;
    deflateStream.zalloc = Z_NULL;
    deflateStream.zfree = Z_NULL;
    deflateStream.opaque = Z_NULL;

    deflateStream.avail_in = inputString.size();
    deflateStream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(inputString.data()));

    // 15 == window bits for sliding window (max for the deflate algorithm)
    // 16 == gzip header
    // 8 == memory level, 1 - 9 , 1 using least amount of memory , 9 using most amount. 9 should be the best compression and most memory and vice versa for 1
    int returnVal = deflateInit2(&deflateStream,Z_BEST_COMPRESSION,Z_DEFLATED,15 | 16,8,Z_DEFAULT_STRATEGY);

    if (returnVal != Z_OK) {
        std::cout << "deflateInit2 failed" << std::endl;
        return inputString;
    }

    char outBuffer[32768];
    std::string compressed;

    do
    {
        deflateStream.avail_out = sizeof(outBuffer);
        deflateStream.next_out = reinterpret_cast<Bytef*>(outBuffer);
        returnVal = deflate(&deflateStream,Z_FINISH);

        if(compressed.size() < deflateStream.total_out)
        {
            compressed.append(outBuffer,deflateStream.total_out - compressed.size());
        }
    } while (returnVal == Z_OK);

    deflateEnd(&deflateStream);

    if(returnVal != Z_STREAM_END)
    {
        std::cout << "deflate failed" << std::endl;
        return inputString;
    }

    return compressed;
}