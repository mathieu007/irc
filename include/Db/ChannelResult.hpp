#pragma once
#include <string>
#include "ResultSet.hpp"

using std::string;
using std::vector;

class ClientResult;
class Channel;
class ClientsChannelsResult;

class ChannelResult : public ResultSet<Channel>
{

public:
    ChannelResult();
    ChannelResult(vector<Channel *> &data);

    Channel *getByName(const string &channelName);
    bool removeByName(const string &channelName);
    bool add(Channel *channel);
    bool exists(const string &channelname) const;

    ClientResult getClients(Channel *channel);
    template <typename TPropValue>
    ChannelResult where(const TPropValue &(Channel::*getter)() const, const TPropValue &value);
    template <typename TPropValue>
    ChannelResult where(TPropValue (Channel::*getter)() const, TPropValue value);
    ClientsChannelsResult getClientsChannels(Channel *channel);
};