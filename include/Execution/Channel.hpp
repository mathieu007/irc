#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "Client.hpp"
#include "Vec.hpp"
#define MAX_CLIENT_PER_CHANNEL 50

using std::string;
using std::vector;
class Channel
{
private:
    string _name;
    string _key;
    string _topic;
    bool _topicIsPublic;
    uint _maxNumClients;
    uint _numClients;
    bool _onInvitation;
    Client *_superModerator;
    Vec<ClientChannelMapping> *_clientsChannelMapping;
    Vec<Client> _invitedClients = Vec<Client>();
    Vec<Client> _bannedClients = Vec<Client>();
    Vec<Client> _moderators = Vec<Client>();

public:
    Channel(string &name, Vec<ClientChannelMapping> *mapping);
    Channel(string &name, string &key, Vec<ClientChannelMapping> *mapping);
    ~Channel();
    const string &getName() const;
    const string &getKey() const;
    void setKey(string key);
    const string &getTopic() const;
    Vec<Client> &getInvitedClients();
    Vec<Client> &getBannedClients();
    Vec<Client> &getModerators();
    uint getMaxNumClients() const;
    uint getNumClients() const;
    void setMaxNumClients(uint maxNumberOfClient);
    void setNumClients(uint numClients);
    Client *getSuperModerator();
    Vec<ClientChannelMapping> getMapping();
    bool deleteModerator(Client *client, Client *moderatorToDelete);
    bool moderatorExist(Client *client);
    bool addModerator(Client *moderator);
    bool isTopicPublic();
    void setTopicPublic(bool topicAsPublic);
    bool hasTopic() const;
    bool isOnInvitationOnly() const;
    // is user in invited list
    bool addBannedClient(Client *client);
    bool addToInvitation(Client *client);
    bool isInIvitationList(Client *client);
    bool isBanned(Client *client);
    bool isModerator(Client *client);
    bool canDeleteChannel(Client *client) const;
    bool canDeleteModerator(Client *client, Client *moderatorToDelete) const;
    void setSuperModerator(Client *moderator);
    void setJoinOnInvitation(bool onInvite);
    void setTopic(string &topic);
    bool removeInvitationList();
    bool removeAllMapping();
    bool removeMapping(Client *client);
};