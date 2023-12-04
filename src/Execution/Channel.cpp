#include <Channel.hpp>
#include "Vector.hpp"
#include "ClientChannelMapping.hpp"

Channel::~Channel() {}

Channel::Channel(string &name, Vec<ClientChannelMapping> *mapping)
{
    _name = name;
    _topic = "";
    _key = "";
    _numClients = 0;
    _maxNumClients = MAX_CLIENT_PER_CHANNEL;
    _topicIsPublic = true;
    _superModerator = nullptr;
    _clientsChannelMapping = mapping;
    _onInvitation = false;
}

Channel::Channel(string &name, string &key, Vec<ClientChannelMapping> *mapping)
{
    _name = name;
    _key = key;
    _topic = "";
    _numClients = 0;
    _maxNumClients = MAX_CLIENT_PER_CHANNEL;
    _topicIsPublic = true;
    _superModerator = nullptr;
    _clientsChannelMapping = mapping;
    _onInvitation = false;
}

Vec<ClientChannelMapping> Channel::getMapping()
{
    Vec<ClientChannelMapping> map = _clientsChannelMapping->where(&ClientChannelMapping::getChannelName, this->getName());
    return map;
}

bool Channel::hasTopic() const
{
    return !this->_topic.empty();
}

void Channel::setTopic(string &topic)
{
    this->_topic = topic;
}

bool Channel::moderatorExist(Client *moderator)
{
    if (!moderator)
        return false;
    ClientChannelMapping *map = getMapping().first(&ClientChannelMapping::getClientUsername, moderator->getUsername());
    if (!map)
        return false;
    return map->getIsModerator() || moderator == _superModerator;
}

void Channel::setJoinOnInvitation(bool onInvite)
{
    this->_onInvitation = onInvite;
}

void Channel::setSuperModerator(Client *moderator)
{
    if (this->_superModerator != nullptr || moderator->isKickedFromChannel(this))
        return;
    this->_superModerator = moderator;
}

bool Channel::addModerator(Client *moderator)
{

    ClientChannelMapping *map = getMapping().first(&ClientChannelMapping::getClientUsername, moderator->getUsername());
    if (!map)
        return false;
    if (map->getIsModerator() == false && !map->getIsBanned())
        map->setIsModerator(true);
    return false;
}

uint Channel::getMaxNumClients() const
{
    return _maxNumClients;
}

uint Channel::getNumClients() const
{
    return _numClients;
}

void Channel::setMaxNumClients(uint maxNumberOfClient)
{
    _maxNumClients = maxNumberOfClient;
}

void Channel::setNumClients(uint numClients)
{
    _numClients = numClients;
}

const string &Channel::getKey() const
{
    return this->_key;
}

void Channel::setKey(string key)
{
    _key = key;
}

const string &Channel::getName() const
{
    return this->_name;
}

const string &Channel::getTopic() const
{
    return this->_topic;
}

Client *Channel::getSuperModerator()
{
    return this->_superModerator;
}

Vec<Client> Channel::getModerators()
{
    return getMapping().where(&ClientChannelMapping::getIsModerator, true).select(&ClientChannelMapping::getClient);
}

bool Channel::isOnInvitationOnly() const
{
    return this->_onInvitation;
}

bool Channel::addToInvitation(Client *client)
{
    if (!client)
        return false;
    ClientChannelMapping *map = getMapping().first(&ClientChannelMapping::getClientUsername, client->getUsername());
    if (_onInvitation && map->getIsInvited() && !map->getIsBanned())
        return false;
    else if (!_onInvitation)
        return false;
    map->setIsInvited(true);
    return true;
}

// check is allowed to join on invitaion only
bool Channel::isAllowedToJoin(Client *client)
{
    if (!client)
        return false;
    ClientChannelMapping *map = getMapping().first(&ClientChannelMapping::getClientUsername, client->getUsername());
    if (!_onInvitation && !map)
        return true;
    if (_onInvitation && map && map->getIsInvited() && !map->getIsBanned() && map->getChannel()->getNumClients() < map->getChannel()->getMaxNumClients())
        return true;
    else if (!_onInvitation && map && !map->getIsBanned() && map->getChannel()->getNumClients() < map->getChannel()->getMaxNumClients())
        return true;
    return false;
}

bool Channel::canDeleteModerator(Client *client, Client *moderatorToDelete) const
{
    if (!moderatorToDelete || !client)
        return false;
    if (client == moderatorToDelete || moderatorToDelete == _superModerator)
        return false;
    return true;
}

bool Channel::deleteModerator(Client *client, Client *moderatorToDelete)
{
    if (!moderatorToDelete || !client)
        return false;
    ClientChannelMapping *mapModerator = getMapping().first(&ClientChannelMapping::getClientUsername, moderatorToDelete->getUsername());
    ClientChannelMapping *mapClient = getMapping().first(&ClientChannelMapping::getClientUsername, client->getUsername());
    if (!mapClient || !mapModerator)
        return false;
    if (canDeleteModerator(client, moderatorToDelete))
        mapModerator->setIsModerator(false);
    return false;
}

bool Channel::isTopicPublic()
{
    return _topicIsPublic;
}

void Channel::setTopicPublic(bool topicAsPublic)
{
    _topicIsPublic = topicAsPublic;
}

bool Channel::removeMapping(Client *client)
{
    getMapping().removeWhere(&ClientChannelMapping::getClientUsername, client->getUsername(), true);
    _clientsChannelMapping->removeNulls();
    return true;
}
