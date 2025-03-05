//
// Created by esmayl on 05/06/2024.
//

#include "Globals.h"

#include <cstdint>

const char Globals::getSuccessResponse[] = "200 OK";
const char Globals::postCreatedResponse[] = "201 Created";
const char Globals::errorResponse[] = "404 Not Found";
const char Globals::plainContentType[] = "text/plain";
const char Globals::octetContentType[] = "application/octet-stream";
const std::string Globals::textFileExtensions[] = {
    ".txt",
    ".json",
    ".html",
    ".php",
    ".js",
    ".ts",
    ".css",
};

const std::vector<std::string> Globals::acceptedEncodings = {"gzip", "deflate", "br"};

HttpHeader Globals::BuildResponse(HttpPacket* packet, const char* headerResponse, const size_t contentLength,const CONTENTTYPE responseType, const bool succes)
{
    HttpHeader header;
    header.responseStatus = headerResponse;
    header.chunked = false;
    header.success = true;

    if(!succes)
    {
        header.responseStatus = Globals::errorResponse;
        header.success = false;
        return header;
    }

    if (contentLength >= 1024) // 1 KB
    {
        header.chunked = true;
    }
    else
    {
        header.contentLength = std::to_string(contentLength);
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

std::vector<uint8_t> Globals::BuildResponseBody(HttpPacket* packet, const std::vector<uint8_t>& inputData, size_t& outLength)
{
    bool isAccepted = false;
    size_t acceptedIndex = -1;

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
            std::vector<uint8_t> compressedData = GzipCompress(inputData);
            outLength = compressedData.size();
            return compressedData;
        }
    }

    printf("Did no compression");

    outLength = inputData.size();
    return inputData;
}

std::vector<uint8_t> Globals::GzipCompress(const std::vector<uint8_t>& inputData)
{
    if (inputData.empty()) return {}; // Return empty if no input

    z_stream deflateStream{};
    deflateStream.zalloc = Z_NULL;
    deflateStream.zfree = Z_NULL;
    deflateStream.opaque = Z_NULL;

    deflateStream.avail_in = static_cast<uInt>(inputData.size());
    deflateStream.next_in = const_cast<Bytef*>(inputData.data());

    // Initialize with gzip format (15 | 16), max compression (Z_BEST_COMPRESSION), and default memory strategy
    int returnVal = deflateInit2(&deflateStream, Z_BEST_COMPRESSION, Z_DEFLATED, 15 | 16, 8, Z_DEFAULT_STRATEGY);
    if (returnVal != Z_OK) {
        printf("deflateInit2 failed\n");
        return {};
    }

    // Allocate buffer with a reasonable initial size
    std::vector<uint8_t> compressed;
    compressed.resize(inputData.size() / 2 + 128);  // Adjust size as needed

    deflateStream.avail_out = static_cast<uInt>(compressed.size());
    deflateStream.next_out = compressed.data();

    // Compression loop to handle large data properly
    do {
        returnVal = deflate(&deflateStream, Z_FINISH);
        if (returnVal == Z_OK || returnVal == Z_BUF_ERROR) {
            // Resize buffer and continue
            size_t currentSize = compressed.size();
            compressed.resize(currentSize * 2); // Double buffer size
            deflateStream.avail_out = static_cast<uInt>(compressed.size() - currentSize);
            deflateStream.next_out = compressed.data() + currentSize;
        }
    } while (returnVal != Z_STREAM_END);

    if (returnVal != Z_STREAM_END) {
        printf("deflate failed\n");
        deflateEnd(&deflateStream);
        return {};
    }

    // Resize to actual compressed size
    compressed.resize(deflateStream.total_out);
    deflateEnd(&deflateStream);

    return compressed;
}
