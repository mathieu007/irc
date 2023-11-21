#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "Client.hpp"
#include "IChannel.hpp"

using std::string;
using std::vector;
class Channel : public IChannel
{
    private:
        string _name;
        vector<Client *> _clients = vector<Client *>(MAX_CLIENTS);
    public:
        Channel(string name);
        ~Channel();
        const string &getChannelName() const;
        bool addClient(Client *client);
        bool isInChannel(Client *client);
};