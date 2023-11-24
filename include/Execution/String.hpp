#pragma once
#include <iostream>
#include <string>

namespace String
{
    bool endsWith(const std::string &fullString, const std::string &suffix);
    bool startWith(const std::string &fullString, const std::string &prefix);
}