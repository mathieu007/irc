#include <Channel.hpp>
#include "ClientChannelMapping.hpp"
#include "Message.hpp"

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
    _invitedClients = Vec<Client>();
    _bannedClients = Vec<Client>();
    _moderators = Vec<Client>();
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
    if (this->_moderators.exist(&Client::getUsername, moderator->getUsername()) || moderator == _superModerator)
        return true;
    return false;
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
    if (!moderator)
        return false;
    if (!this->_moderators.exist(&Client::getUsername, moderator->getUsername()))
    {
        _moderators.push_back(moderator);
        return true;
    }
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

Vec<Client> &Channel::getModerators()
{
    return _moderators;
}

Vec<Client> &Channel::getBannedClients()
{
    return _bannedClients;
}

Vec<Client> &Channel::getInvitedClients()
{
    return _invitedClients;
}

bool Channel::isOnInvitationOnly() const
{
    return this->_onInvitation;
}

bool Channel::addBannedClient(Client *client)
{
    if (!client)
        return false;
    if (!this->_bannedClients.exist(&Client::getUsername, client->getUsername()))
    {
        _bannedClients.push_back(client);
        return true;
    }
    return false;
}

bool Channel::addToInvitation(Client *client)
{
    if (!client)
        return false;
    if (_onInvitation && !this->_invitedClients.exist(&Client::getUsername, client->getUsername()))
    {
        _invitedClients.push_back(client);
        _bannedClients.removeWhere(&Client::getUsername, client->getUsername(), false);
        return true;
    }
    return false;
}

bool Channel::isBanned(Client *client)
{
    if (!client)
        return false;
    if (this->_bannedClients.exist(&Client::getUsername, client->getUsername()))
        return true;
    return false;
}

bool Channel::isModerator(Client *client)
{
    if (!client)
        return false;
    if (this->_moderators.exist(&Client::getUsername, client->getUsername()) || this->_superModerator == client)
        return true;
    return false;
}

// check is allowed to join on invitaion only
bool Channel::isInIvitationList(Client *client)
{
    if (!client)
        return false;
    if (!_onInvitation)
        return false;
    if (_onInvitation && this->_invitedClients.exist(&Client::getUsername, client->getUsername()))
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
    if (canDeleteModerator(client, moderatorToDelete))
        _moderators.removeWhere(&Client::getUsername, moderatorToDelete->getUsername(), false);
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

bool Channel::removeInvitationList()
{
    return _invitedClients.removeAll(false);
}

bool Channel::sendMsgToAll(string &msg)
{
    Vec<Client> clients = getMapping().select(&ClientChannelMapping::getClient);
    for (size_t i = 0; i < clients.size(); i++)
    {
        Msg::sendMsg(clients[i], msg, 0);
    }
    return true;
}
