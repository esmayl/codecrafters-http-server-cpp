//
// Created by esmayl on 15/08/2024.
//

#include "StringUtils.h"

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
