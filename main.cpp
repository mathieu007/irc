#include <Server.hpp>

int main(int argc, char **argv)
{
    if (argc != 3)
        throw std::runtime_error("Error: invalid number of arguments should be: ./ircserv <port> <password>\n");
}