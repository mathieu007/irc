#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "Client.hpp"

using std::string;
using std::vector;
class Channel
{
private:
    string _id;
    string _name;
    string _key;
    string _topic;
    bool _canBeJoinOnInvitationOnly;
    Client *_superModerator;
    vector<Client *> _moderators;
    vector<Client *> _invitedClients;

public:
    Channel(string &name);
    Channel(string &name, string &key);
    ~Channel();
    const string &getName() const;
    const string &getKey() const;
    const string &getId() const;
	const string &getTopic() const;
    Client *getSuperModerator();
    vector<Client *> &getModerators();
    vector<Client *> &getInvitedClients();
    bool deleteModerator(Client *client, Client *moderatorToDelete);
    bool moderatorAlreadyExist(Client *client) const;
    bool addModerator(Client *moderator);
    bool hasTopic() const;
    bool canBejoinOnInvitationOnly() const;
    bool isAllowedToJoin(Client *client) const;
    bool canDeleteChannel(Client *client) const;
    bool canDeleteModerator(Client *client, Client *moderatorToDelete) const;
    void setSuperModerator(Client *moderator);
    void setJoinOnInvitationOnly(bool onInvite);
    void setTopic(string &topic);
};