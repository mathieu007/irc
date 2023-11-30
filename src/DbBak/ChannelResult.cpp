#include "Db.hpp"
#include "ChannelResult.hpp"
#include "ClientResult.hpp"
#include "Channel.hpp"
#include "ClientsChannelsResult.hpp"
#include "ClientChannelMapping.hpp"
#include "Vector.hpp"

using std::string;

ChannelResult::ChannelResult() : ResultSet()
{
}
ChannelResult::ChannelResult(vector<Channel *> &data)
{
    _data = data;
}

Channel *ChannelResult::getByName(const string &channelName)
{
    return Vector::getOne(_data, &Channel::getName, channelName);
}

bool ChannelResult::removeByName(const string &channelName)
{
    return Vector::removeWhere(_data, &Channel::getName, channelName);
}

bool ChannelResult::add(Channel *channel)
{
    if (this->exists(channel->getName()))
        return false;
    _data.push_back(channel);
    return true;
}
bool ChannelResult::exists(const string &channelName) const
{
    return Vector::exist(_data, &Channel::getName, channelName);
}

ClientResult ChannelResult::getClients(Channel *channel)
{
    ClientsChannelsResult clientsChannels = Db::clientsChannels.where(&ClientChannelMapping::getChannelName, channel->getName());
    return clientsChannels.getClients();
}

template <typename TPropValue>
ChannelResult ChannelResult::where(const TPropValue &(Channel::*getter)() const, const TPropValue &value)
{
    vector<Channel *> res = Vector::where(_data, getter, value);
    return ChannelResult(res);
}

template <typename TPropValue>
ChannelResult ChannelResult::where(TPropValue (Channel::*getter)() const, TPropValue value)
{
    vector<Channel *> res = Vector::where(_data, getter, value);
    return ChannelResult(res);
}
ClientsChannelsResult ChannelResult::getClientsChannels()
{
    
    ClientsChannelsResult clientsChannels = Db::clientsChannels.where(&ClientChannelMapping::getChannelName, channel->getName());
    return clientsChannels;
}
