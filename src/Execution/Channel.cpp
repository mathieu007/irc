#include <Channel.hpp>

Channel::~Channel() {}

Channel::Channel(string &name)
{
    _name = name;
    _id = name;
}

Channel::Channel(string &name, string &key)
{
    _name = name;
    _key = key;
    _id = _name + ":" + _key;
}

bool Channel::hasTopic() const
{
    return this->_hasTopic;
}

void Channel::setTopic(string &topic)
{
    this->_topic = topic;
}

const string &Channel::getId() const
{
    return this->_id;
}

const string &Channel::getKey() const
{
    return this->_key;
}

const string &Channel::getName() const
{
    return this->_name;
}

// Map<string, Client *> &Channel::getClients()
// {
//     return this->_clients;
// }

// Client *Channel::getClient(string userName)
// {
//     Client *client = nullptr;
//     this->_clients.tryGet(userName, client);
//     return client;
// }

// bool Channel::addClient(Client *client)
// {
//     string username = client->getUsername();
//     if (_clients.addIfNotExist(username, client))
//         return false;
//     return true;
// }

// bool Channel::isInChannel(Client *client)
// {
//     if (!client)
//         return false;
//     string username = client->getUsername();
//     if (_clients.hasKey(username))
//         return true;
//     return false;
// }

// bool Channel::isInChannel(string &username)
// {
//     if (_clients.hasKey(username))
//         return true;
//     return false;
// }
