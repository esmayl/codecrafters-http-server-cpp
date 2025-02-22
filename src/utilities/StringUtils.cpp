//
// Created by esmayl on 15/08/2024.
//

#include "StringUtils.h"

#include <charconv>

// Function to trim whitespace from the beginning of a string
std::string StringUtils::LeftTrim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    return (start == std::string::npos) ? "" : str.substr(start);
}

// Function to trim whitespace from the end of a string
std::string StringUtils::RightTrim(const std::string& str) {
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

// Function to trim whitespace from both ends of a string
std::string StringUtils::Trim(const std::string& str) {
    return RightTrim(LeftTrim(str));
}

std::vector<std::string> StringUtils::SplitString(const std::string& str, char delimiter)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        result.push_back(Trim(item));
    }

    return result;
}

std::string StringUtils::DecodeUrl(const std::string &encoded){
    std::string decoded;
    decoded.reserve(encoded.length()); // Reserve memory to avoid multiple reallocations

    for (size_t i = 0; i < encoded.length(); ++i) {
        if (encoded[i] == '%' && i + 2 < encoded.length()) {
            char hex[3] = { encoded[i + 1], encoded[i + 2], '\0' };
            int charCode;
            if (std::from_chars(hex, hex + 2, charCode, 16).ec == std::errc()) {
                decoded += static_cast<char>(charCode);
                i += 2;
                continue;
            }
        } else if (encoded[i] == '+') {
            decoded += ' '; // '+' in URLs represents a space
        } else {
            decoded += encoded[i];
        }
    }
    return decoded;
}