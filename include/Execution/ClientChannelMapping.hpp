#pragma once
#include <string>
#include <vector>
#include <Map.hpp>

using std::string;
using std::vector;

class Client;
class Channel;
// class ClientChannelMapping;

class ClientChannelMapping
{
private:
    Client *_client;
    Channel *_channel;

public:
    ClientChannelMapping(Client *client, Channel *channel);
    bool operator==(ClientChannelMapping &cmp);
    bool operator!=(ClientChannelMapping &cmp);
    ~ClientChannelMapping();

    Channel *getChannel();
    Client *getClient();

    const string &getChannelName() const;
    const string &getClientUsername() const;
    uint getChannelClientCount() const;

    bool getIsModerator() const;
    bool getIsInvited() const;
    bool getIsBanned() const;

    void setIsModerator(bool moderator);
    void setIsInvited(bool moderator);
    void setIsBanned(bool moderator);
};