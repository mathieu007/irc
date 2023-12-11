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

    bool contains(const std::string &mainString, const std::string &substring)
    {
        return mainString.find(substring) != std::string::npos;
    }

    std::string extractUptoFirstOccurence(const std::string &input, const std::string &substring, bool excludeOcc)
    {
        size_t pos = input.find(substring);

        if (pos != std::string::npos)
        {
            if (excludeOcc)
                return input.substr(0, pos);
            else
                return input.substr(0, pos + substring.length());
        }
        else
        {
            return "";
        }
    }

    bool startWith(const std::string &fullString, const std::string &prefix)
    {
        if (fullString.length() >= prefix.length())
            return (fullString.compare(0, prefix.length(), prefix) == 0);
        return false;
    }
}