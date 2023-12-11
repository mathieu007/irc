#pragma once
#include <iostream>
#include <string>

namespace String
{
    bool endsWith(const std::string &fullString, const std::string &suffix);
    std::string extractUptoFirstOccurence(const std::string &input, const std::string &substring, bool excludeOccurence);
    bool startWith(const std::string &fullString, const std::string &prefix);
    bool contains(const std::string &mainString, const std::string &substring);
}