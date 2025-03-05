//
// Created by esmayl on 03/03/2025.
//

#ifndef GZIPSTREAM_H
#define GZIPSTREAM_H

#pragma once
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <vector>
#include "zlib.h"


// Persistent Gzip Compression Context
class GzipStream
{
    public:
        GzipStream();
        ~GzipStream();
        std::vector<uint8_t> compress(const std::vector<uint8_t>& input, bool flush);

    private:
        z_stream deflateStream{};
};



#endif //GZIPSTREAM_H
