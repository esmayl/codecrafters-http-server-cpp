//
// Created by esmayl on 05/06/2024.
//

#include "Globals.h"

const char Globals::getSuccessResponse[] = "200 OK\r\n";
const char Globals::postCreatedResponse[] = "201 Created\r\n";
const char Globals::errorResponse[] = "404 Not Found\r\n";
const char Globals::plainContentType[] = "text/plain\r\n";
const char Globals::octetContentType[] = "application/octet-stream\r\n";

const std::vector<std::string> Globals::acceptedEncodings = {"gzip", "deflate", "br"};

HttpHeader Globals::BuildResponse(HttpPacket* packet, const char* headerResponse, const size_t contentLength,const CONTENTTYPE responseType, const bool succes)
{
    HttpHeader header;
    header.responseStatus = headerResponse;
    header.chunked = false; // Just set this initially, can always be overwritten when sending a big file

    if(!succes)
    {
        header.responseStatus = Globals::errorResponse;
        header.success = false;
        return header;
    }

    if (contentLength > 1048576) // 1 MB
    {
        header.chunked = true;
    }

    bool isAccepted = false;
    int acceptedIndex = -1;


    if(!packet->GetContentEncoding()->empty())
    {
        std::vector<std::string>* requestEncoding = packet->GetContentEncoding();

        for(const std::string& str: acceptedEncodings)
        {
            size_t requestSize = requestEncoding->size();

            for(int i=0;i<requestSize;i++)
            {
                if(str == requestEncoding->at(i))
                {
                    isAccepted = true;
                    acceptedIndex = i;
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
            header.contentEncoding = acceptedEncodings[acceptedIndex];
            printf("Building response with content encoding: ");
            printf(header.contentEncoding.c_str());
            printf("\n");
        }
    }

    switch (responseType)
    {
        case CONTENTTYPE::PLAIN:
            header.contentType = Globals::plainContentType;
            break;
        case CONTENTTYPE::OCTET:
            header.contentType = Globals::octetContentType;
            break;
    }

    header.contentLength = std::to_string(contentLength);

    printf("Build response:\n\n");
    printf(header.ToString().c_str());

    // if(!responseBody.empty())
    // {
    //     buildResponse.append(responseBody);
    //     printf("[Gzip compressed body, content length: ");
    //     printf(std::to_string(responseBody.length()).c_str());
    //     printf("]\n\n");
    // }
    // else
    // {
    //     buildResponse.append("\r\n");
    //     printf("\n\n");
    // }

    return header;
}

char* Globals::BuildResponseBody(HttpPacket* packet, const char* responseBody, size_t& outLength)
{
    bool isAccepted = false;
    int acceptedIndex = -1;
    char* compressedBody = nullptr;

    if (!packet->GetContentEncoding()->empty()) {
        std::vector<std::string>* requestEncoding = packet->GetContentEncoding();

        for (size_t i = 0; i < acceptedEncodings.size(); i++) {
            for (const std::string& encoding : *requestEncoding) {
                if (acceptedEncodings[i] == encoding) {
                    isAccepted = true;
                    acceptedIndex = i;
                    break;
                }
            }
            if (isAccepted) break;
        }

        if (isAccepted && acceptedEncodings[acceptedIndex] == "gzip") {
            std::string compressedStr = GzipCompress(std::string(responseBody));
            outLength = compressedStr.size();
            compressedBody = new char[outLength + 1];
            std::memcpy(compressedBody, compressedStr.c_str(), outLength);
            compressedBody[outLength] = '\0';
            return compressedBody;
        }
    }

    outLength = std::strlen(responseBody);
    compressedBody = new char[outLength + 1];
    std::memcpy(compressedBody, responseBody, outLength);
    compressedBody[outLength] = '\0';
    return compressedBody;
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
        printf("deflateInit2 failed");
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
        printf("deflate failed");
        return inputString;
    }

    return compressed;
}