//
// Created by esmayl on 03/03/2025.
//

#include "GzipStream.h"

GzipStream::GzipStream()
{
    deflateStream.zalloc = Z_NULL;
    deflateStream.zfree = Z_NULL;
    deflateStream.opaque = Z_NULL;

    // Initialize gzip with 15 | 16 for gzip header
    if (deflateInit2(&deflateStream, Z_BEST_COMPRESSION, Z_DEFLATED, 15 | 16, 8, Z_DEFAULT_STRATEGY) != Z_OK)
    {
        printf("deflateInit2 failed\n");
        throw std::runtime_error("Failed to initialize gzip stream");
    }
}

GzipStream::~GzipStream()
{
    deflateEnd(&deflateStream);
}

std::vector<uint8_t> GzipStream::compress(const std::vector<uint8_t>& input, bool flush)
{
    if (input.empty()) return {};

    deflateStream.avail_in = static_cast<uInt>(input.size());
    deflateStream.next_in = const_cast<Bytef*>(input.data());

    std::vector<uint8_t> compressed(input.size() * 2 + 128);
    deflateStream.avail_out = static_cast<uInt>(compressed.size());
    deflateStream.next_out = compressed.data();

    int ret = deflate(&deflateStream, flush ? Z_FINISH : Z_SYNC_FLUSH);
    if (ret != Z_OK && ret != Z_STREAM_END) {
        printf("deflate failed\n");
        return {};
    }

    // Resize to actual compressed data size
    compressed.resize(compressed.size() - deflateStream.avail_out);
    return compressed;
}
