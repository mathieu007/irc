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
    _isSuperModerator = false;
}

bool ClientChannelMapping::operator==(const ClientChannelMapping &cmp) const
{
    return (cmp.getChannel()->getName() == this->getChannel()->getName() && cmp.getClient()->getUsername() == this->getClient()->getUsername());
}
bool ClientChannelMapping::operator!=(const ClientChannelMapping &cmp) const
{
    return !(cmp.getChannel()->getName() == this->getChannel()->getName() && cmp.getClient()->getUsername() == this->getClient()->getUsername());
}
ClientChannelMapping::~ClientChannelMapping() {}

Channel *ClientChannelMapping::getChannel() const
{
    return _channel;
}
Client *ClientChannelMapping::getClient() const
{
    return _client;
}

bool ClientChannelMapping::getIsModerator() const
{
    return _isModerator;
}
bool ClientChannelMapping::getIsSuperModerator() const
{
    return _isSuperModerator;
}
bool ClientChannelMapping::getIsInvited() const
{
    return _isInvited;
}

void ClientChannelMapping::setIsModerator(bool moderator)
{
    _isModerator = moderator;
}
void ClientChannelMapping::setIsSuperModerator(bool supermoderator)
{
    _isSuperModerator = supermoderator;
}
void ClientChannelMapping::setIsInvited(bool invited)
{
    _isInvited = invited;
}
