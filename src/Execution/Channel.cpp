#include <Channel.hpp>

Channel::~Channel(){}

Channel::Channel(string name)
{
    _name = name;
}

const string &Channel::getChannelName() const
{
    return this->_name;
}

bool Channel::addClient(Client *client)
{
    if (!client || client->getSocket() > MAX_CLIENTS || _clients[client->getSocket()]  != NULL)
        return false;
    _clients[client->getSocket()] = client;
    return true;
}

bool Channel::isInChannel(Client *client)
{
    if (!client)
        return false;
    if ((size_t)client->getSocket() >= _clients.size() || _clients[client->getSocket()] == NULL)
        return false;
    return true;
}