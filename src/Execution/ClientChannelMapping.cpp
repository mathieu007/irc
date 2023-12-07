#include <string>
#include "ClientChannelMapping.hpp"
#include "Channel.hpp"
#include "Client.hpp"

using std::string;
using std::vector;

ClientChannelMapping::ClientChannelMapping(Client *client, Channel *channel)
{
    _client = client;
    _channel = channel;
}

bool ClientChannelMapping::operator==(ClientChannelMapping &cmp)
{
    return (cmp.getChannel()->getName() == this->getChannel()->getName() && cmp.getClient()->getUsername() == this->getClient()->getUsername());
}
bool ClientChannelMapping::operator!=(ClientChannelMapping &cmp)
{
    return !(cmp.getChannel()->getName() == this->getChannel()->getName() && cmp.getClient()->getUsername() == this->getClient()->getUsername());
}
ClientChannelMapping::~ClientChannelMapping() {}

Channel *ClientChannelMapping::getChannel()
{
    return _channel;
}
uint ClientChannelMapping::getChannelClientCount() const
{
    return _channel->getNumClients();
}

const string &ClientChannelMapping::getChannelName() const
{
    return _channel->getName();
}
const string &ClientChannelMapping::getClientUsername() const
{
    return _client->getUsername();
}

Client *ClientChannelMapping::getClient()
{
    return _client;
}
bool ClientChannelMapping::getIsModerator() const
{
    return _channel;
}
bool ClientChannelMapping::getIsInvited() const
{
    Client *client = _channel->getInvitedClients().first(&Client::getUsername, _client->getUsername());
    if (!client)
        return false;
    return true;
}
bool ClientChannelMapping::getIsBanned() const
{
    Client *client = _channel->getBannedClients().first(&Client::getUsername, _client->getUsername());
    if (!client)
        return false;
    return true;
}

void ClientChannelMapping::setIsModerator(bool moderator)
{
    Client *client = _channel->getModerators().first(&Client::getUsername, _client->getUsername());
    if (!client && moderator)
        _channel->getModerators().push_back(_client);
    if (client && !moderator)
        _channel->getModerators().removeWhere(&Client::getUsername, _client->getUsername(), false);
}
void ClientChannelMapping::setIsInvited(bool invited)
{
    Client *client = _channel->getModerators().first(&Client::getUsername, _client->getUsername());
    if (!client && invited)
        _channel->getInvitedClients().push_back(_client);
    if (client && !invited)
        _channel->getInvitedClients().removeWhere(&Client::getUsername, _client->getUsername(), false);
}

void ClientChannelMapping::setIsBanned(bool banned)
{
    Client *client = _channel->getModerators().first(&Client::getUsername, _client->getUsername());
    if (!client && banned)
        _channel->getBannedClients().push_back(_client);
    if (client && !banned)
        _channel->getBannedClients().removeWhere(&Client::getUsername, _client->getUsername(), false);
}
