#pragma once
#include <string>
using std::string;

class Client;

/// it's really yuck but using interface to avoid circular references
class IChannel  
{
    public:
        virtual const string &getChannelName() const = 0;
        virtual bool addClient(Client *client) = 0;
        virtual bool isInChannel(Client *client) = 0;
};