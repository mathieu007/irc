#include "Db.hpp"
#include "ClientsChannelsResult.hpp"
#include "ChannelResult.hpp"
#include "ClientResult.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "ClientChannelMapping.hpp"
#include "Vector.hpp"

using std::vector;

ClientsChannelsResult::ClientsChannelsResult() : MappingResultSet()
{
}


ClientsChannelsResult::ClientsChannelsResult(vector<ClientChannelMapping *> &data)
{
    _data = data;
}

ClientsChannelsResult::ClientsChannelsResult()
{
    _data = vector<ClientChannelMapping *>();
}

ChannelResult ClientsChannelsResult::getChannels()
{
    vector<Channel *> vec = vector<Channel *>();
    for (std::map<string, ClientChannelMapping *>::iterator it = _data.begin(); it != _data.end(); ++it)
    {
        ClientChannelMapping *value = (*it).second;
        it++;
        for (std::map<string, ClientChannelMapping *>::iterator it = _data.begin(); it != _data.end(); ++it)
        {
            ClientChannelMapping *value2 = (*it).second;
            if (value->getChannel() != value2->getChannel())
            {
                it--;
                break;
            }
        }
        vec.push_back(value->getChannel());
    }
    return ChannelResult(vec);
}

ClientResult ClientsChannelsResult::getClients()
{
    vector<Client *> vec = vector<Client *>();
    for (std::map<string, ClientChannelMapping *>::iterator it = _data.begin(); it != _data.end(); ++it)
    {
        ClientChannelMapping *value = (*it).second;
        it++;
        for (std::map<string, ClientChannelMapping *>::iterator it = _data.begin(); it != _data.end(); ++it)
        {
            ClientChannelMapping *value2 = (*it).second;
            if (value->getClient() != value2->getClient())
            {
                it--;
                break;
            }
        }
        vec.push_back(value->getClient());
    }
    return ClientResult(vec);
}

ChannelResult ClientsChannelsResult::getChannelsByClient(Client *client)
{
    string keySearch = client->getUsername() + ":";
    _data.
}

ClientResult ClientsChannelsResult::getClients(Channel *channel)
{
    vector<ClientChannelMapping *> res = Vector::where(_data, &ClientChannelMapping::getChannelName, channel->getName());
    vector<Client *> sel = Vector::select(res, &ClientChannelMapping::getClient);
    return ClientResult(sel);
}

bool ClientsChannelsResult::add(Client *client, Channel *channel)
{
    ClientsChannelsResult res = this->where(&Client::getUsername, client->getUsername()).where(&Channel::getName, channel->getName());
    if (res.hasResult())
        return false;
    ClientChannelMapping *map = new ClientChannelMapping(client, channel);
    _data.addOrReplace(getKey(*map), map);
    return true;
}

bool ClientsChannelsResult::remove(ClientChannelMapping *item)
{
    return Vector::remove(_data, item);
}

bool ClientsChannelsResult::remove(Client *client, Channel *channel)
{
    ClientsChannelsResult res = this->where(&Client::getUsername, client->getUsername()).where(&Channel::getName, channel->getName());
    if (!res.hasResult())
        return false;
    ClientChannelMapping *map = res.one();
    return remove(map);
}

bool ClientsChannelsResult::removeAllByClient(Client *client)
{
    return Vector::removeWhere(_data, &ClientChannelMapping::getClientUsername, client->getUsername());
}

bool ClientsChannelsResult::removeAllByChannel(Channel *channel)
{
    return Vector::removeWhere(_data, &ClientChannelMapping::getClientUsername, channel->getName());
}

bool ClientsChannelsResult::exits(Client *client, Channel *channel)
{
    ClientsChannelsResult res = this->where(&Client::getUsername, client->getUsername()).where(&Channel::getName, channel->getName());
    return res.hasResult();
}

bool ClientsChannelsResult::exits(const string &username, const string &channelname)
{
    ClientsChannelsResult res = this->where(&Client::getUsername, username).where(&Channel::getName, channelname);
    return res.hasResult();
}

// you can chain result set...
// template <typename TPropValue>
// ClientsChannelsResult ClientsChannelsResult::where(TPropValue (Channel::*getter)() const, TPropValue value)
// {
//     vector<ClientChannelMapping *> res = Vector::where(_data, getter, value);
//     return ClientsChannelsResult(res);
// }

template <typename TPropValue>
ClientsChannelsResult ClientsChannelsResult::where(TPropValue &(Channel::*getter)() const, TPropValue &value)
{
    vector<Channel *> chans = Vector::select(_data, &ClientsChannelsResult::getChannels());
    ChannelResult res = ChannelResult(chans);
    res = res.where(getter, value);
    return ClientsChannelsResult(res);
}

// you can chain result set...
// template <typename TPropValue>
// ClientsChannelsResult ClientsChannelsResult::where(TPropValue (Client::*getter)() const, TPropValue value)
// {
//     vector<ClientChannelMapping *> res = Vector::where(_data, getter, value);
//     return ClientsChannelsResult(res);
// }

template <typename TPropValue>
ClientsChannelsResult ClientsChannelsResult::where(const TPropValue &(Client::*getter)() const, const TPropValue &value)
{
    vector<ClientChannelMapping *> res = Vector::where(_data, getter, value);
    return ClientsChannelsResult(res);
}
