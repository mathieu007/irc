#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "Client.hpp"

using std::string;
using std::vector;

class Channel 
{
    private:
        string _name;
        vector<Client> _clients;
};