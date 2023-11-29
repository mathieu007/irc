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
    bool _topicPublic;
    uint _maxNumClients;
    uint _numClients;
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
    void setKey(string key);
    const string &getId() const;
    const string &getTopic() const;
    uint getMaxNumClients() const;
    uint getNumClients() const;
    void setMaxNumClients(uint maxNumberOfClient);
    void setNumClients(uint numClients);
    Client *getSuperModerator();
    vector<Client *> &getModerators();
    vector<Client *> &getInvitedClients();
    bool deleteModerator(Client *client, Client *moderatorToDelete);
    bool moderatorAlreadyExist(Client *client) const;
    bool addModerator(Client *moderator);
    bool isTopicPublic();
    void setTopicPublic(bool topicAsPublic);
    bool hasTopic() const;
    bool isOnInvitationOnly() const;
    // is user in invited list
    bool isAllowedToJoin(Client *client) const;
    bool canDeleteChannel(Client *client) const;
    bool canDeleteModerator(Client *client, Client *moderatorToDelete) const;
    void setSuperModerator(Client *moderator);
    void setJoinOnInvitationOnly(bool onInvite);
    void setTopic(string &topic);
};