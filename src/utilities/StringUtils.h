//
// Created by esmayl on 15/08/2024.
//

#ifndef STRINGUTILS_H
#define STRINGUTILS_H
#include <string>
#include <vector>
#include <sstream>


class StringUtils {
    public:
        static std::string LeftTrim(const std::string& str);
        static std::string RightTrim(const std::string& str);
        static std::string Trim(const std::string& str);
        static std::vector<std::string> SplitString(const std::string& str, char delimiter);
};



#endif //STRINGUTILS_H
