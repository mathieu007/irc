#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "Client.hpp"
#define MAX_CLIENTS FD_SETSIZE

using std::string;
using std::vector;

class Client;

class Channel 
{
    private:
        string _name;
        vector<Client *> _clients;
    public:
        Channel(string name);
        ~Channel();
        bool addClient(Client *client);
        bool isInChannel(Client *client);
};