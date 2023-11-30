#pragma once
#include <string>
#include <vector>

using std::vector;
using std::string;

class Client;
class Channel;
// class ClientChannelMapping;

class ClientChannelMapping
{
private:
    Client *_client;
    Channel *_channel;
    bool _isModerator;
    bool _isSuperModerator;
    bool _isInvited;

public:
    ClientChannelMapping(Client *client, Channel *channel);
    bool operator==(const ClientChannelMapping &cmp) const;
    bool operator!=(const ClientChannelMapping &cmp) const;
    ~ClientChannelMapping();

    Channel *getChannel() const;
    Client *getClient() const;

    const string &getChannelName() const;
    const string &getClientUsername() const;

    bool getIsModerator() const;
    bool getIsSuperModerator() const;
    bool getIsInvited() const;

    void setIsModerator(bool moderator);
    void setIsSuperModerator(bool moderator);
    void setIsInvited(bool moderator);
};