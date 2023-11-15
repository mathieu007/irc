#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

using std::string;
using std::vector;

class Client;

class Channel 
{
    private:
        string _name;
        vector<Client *> _clients;
        vector<Client *> _banChannel;
};