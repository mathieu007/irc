#include <Channel.hpp>
#include "Vector.hpp"

Channel::~Channel() {}

Channel::Channel(string &name)
{
    _name = name;
    _id = name;
    _topic = "";
    _key = "";
    _superModerator = nullptr;
    _moderators = vector<Client *>();
}

Channel::Channel(string &name, string &key)
{
    _name = name;
    _key = key;
    _id = _name;
    _topic = "";
    _superModerator = nullptr;
    _moderators = vector<Client *>();
}

bool Channel::hasTopic() const
{
    return !this->_topic.empty();
}

void Channel::setTopic(string &topic)
{
    this->_topic = topic;
}

bool Channel::moderatorAlreadyExist(Client *moderator) const
{
    if (!moderator)
        return false;
    if (Vector::isIn(this->_moderators, *moderator, &Client::getUsername))
        return true;
    return false;
}

void Channel::setJoinOnInvitationOnly(bool onInvite)
{
    this->_canBeJoinOnInvitationOnly = onInvite;
}

void Channel::setSuperModerator(Client *moderator)
{
    if (this->_superModerator != nullptr)
        return;
    this->_superModerator = moderator;
    if (!moderatorAlreadyExist(moderator))
        _moderators.push_back(moderator);
}

bool Channel::addModerator(Client *moderator)
{
    if (!moderatorAlreadyExist(moderator))
    {
        _moderators.push_back(moderator);
        return true;
    }
    return false;
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

const string &Channel::getTopic() const
{
    return this->_topic;
}

Client *Channel::getSuperModerator()
{
    return this->_superModerator;
}

vector<Client *> &Channel::getModerators()
{
    return this->_moderators;
}

bool Channel::canBejoinOnInvitationOnly() const
{
    return this->_canBeJoinOnInvitationOnly;
}

bool Channel::isAllowedToJoin(Client *client) const
{
    if (!client)
        return false;
    if (_canBeJoinOnInvitationOnly && Vector::isIn(_invitedClients, *client, &Client::getUsername))
        return true;
    else if (!_canBeJoinOnInvitationOnly)
        return true;
    return false;
}

bool Channel::canDeleteModerator(Client *client, Client *moderatorToDelete) const
{
    if (!moderatorToDelete || client)
        return false;
    if (client->getUsername() == moderatorToDelete->getUsername() || moderatorToDelete->getUsername() == _superModerator->getUsername())
        return false;
    return true;
}

bool Channel::deleteModerator(Client *client, Client *moderatorToDelete)
{
    if (!moderatorToDelete || client)
        return false;
    string username = moderatorToDelete->getUsername();
    if (canDeleteModerator(client, moderatorToDelete))
        return Vector::removeWhere(_moderators, &Client::getUsername, username);
    return false;
}
