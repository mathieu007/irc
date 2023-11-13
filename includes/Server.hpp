
#include <string>
#include <sstream>
#include <iostream>
#include <fcntl.h>
#include <stdexcept>
#include <vector>
#include "Channel.hpp"

using std::string;
using std::vector;
class Server
{
    string _host;
    string _port;
    string _pass;
    vector<Channel *> _channels;
    // vector<Client *> _clients;
};