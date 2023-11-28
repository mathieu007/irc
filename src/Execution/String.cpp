#include <iostream>
#include <string>

namespace String
{
    bool endsWith(const std::string &fullString, const std::string &suffix)
    {
        if (fullString.length() >= suffix.length())
            return (fullString.compare(fullString.length() - suffix.length(), suffix.length(), suffix) == 0);
        return false;
    }
    bool startWith(const std::string &fullString, const std::string &prefix)
    {
        if (fullString.length() >= prefix.length())
            return (fullString.compare(0, prefix.length(), prefix) == 0);
        return false;
    }
}