#include <iostream>
#include <fstream>

class Logger
{
private:
    std::ofstream _logFile;

public:
    Logger();
    void close();
};
