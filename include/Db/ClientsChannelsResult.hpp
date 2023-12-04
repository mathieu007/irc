#pragma once
#include <string>
#include "ResultSet.hpp"

using std::string;
using std::vector;

class ClientChannelMapping;
class Client;
class Channel;

class ClientsChannelsResult: public ResultSet<ClientChannelMapping>
{
public:
    ClientsChannelsResult();
    ClientsChannelsResult(Map<string, ClientChannelMapping *> &data);
    ClientsChannelsResult(vector<ClientChannelMapping *> &data);
    string &getKey(ClientChannelMapping &map) const;

    ChannelResult getChannels();
    ClientResult getClients();

    ChannelResult getChannelsByClient(Client *client);
    ClientResult getClients(Channel *channel);

    bool add(Client *client, Channel *channel);
    bool remove(ClientChannelMapping *item);
    bool remove(Client *client, Channel *channel);
    bool removeAllByClient(Client *client);
    bool removeAllByChannel(Channel *channel);
    bool exits(Client *client, Channel *channel);
    bool exits(const string &username, const string &channelname);

    // you can chain result set...
    // template <typename TPropValue>
    // ClientsChannelsResult where(TPropValue (Channel::*getter)() const, TPropValue value);
    template <typename TPropValue>
    ClientsChannelsResult where(TPropValue &(Channel::*getter)() const, TPropValue &value);

    // you can chain result set...
    // template <typename TPropValue>
    // ClientsChannelsResult where(TPropValue (Client::*getter)() const, TPropValue value);
    template <typename TPropValue>
    ClientsChannelsResult where(const TPropValue &(Client::*getter)() const, const TPropValue &value);
};
