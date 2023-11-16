#include <iostream>
#include <string>

namespace stringUtils
{
    bool endsWith(const std::string &fullString, const std::string &suffix)
    {
        if (fullString.length() >= suffix.length())
        {
            return (fullString.compare(fullString.length() - suffix.length(), suffix.length(), suffix) == 0);
        }
        else
        {
            return false;
        }
    }
}