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
    _isInvited = false;
    _isModerator = false;
    _isBanned = false;
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
    return _isModerator;
}
bool ClientChannelMapping::getIsInvited() const
{
    return _isInvited;
}
bool ClientChannelMapping::getIsBanned() const
{
    return _isBanned;
}

void ClientChannelMapping::setIsModerator(bool moderator)
{
    _isModerator = moderator;
}
void ClientChannelMapping::setIsInvited(bool invited)
{
    _isInvited = invited;
}
void ClientChannelMapping::setIsBanned(bool banned)
{
    _isBanned = banned;
}
