
#include "Db.hpp"
#include "ClientResult.hpp"
#include "ResultSet.hpp"
#include "ChannelResult.hpp"
#include "ClientsChannelsResult.hpp"
#include "Client.hpp"
#include "ClientChannelMapping.hpp"
#include "Vector.hpp"

using std::string;

ClientResult::ClientResult() : ResultSet()
{
}

ClientResult::ClientResult(vector<Client *> &data)
{
    _data = data;
}

Client *ClientResult::getByUserName(const string &userName)
{
    return Vector::getOne(_data, &Client::getNickname, userName);
}

Client *ClientResult::getByNickName(const string &nickName)
{
    return Vector::getOne(_data, &Client::getNickname, nickName);
}

bool ClientResult::removeByUserName(const string &userName)
{
    return Vector::removeWhere(_data, &Client::getUsername, userName);
}

bool ClientResult::remove(Client *client)
{
    ClientResult res = this->where(&Client::getUsername, client->getUsername());
    if (!res.hasResult())
        return false;
    Client *map = res.one();
    return remove(map);
}

bool ClientResult::add(Client *client)
{
    if (this->exists(client->getUsername()))
        return false;
    _data.push_back(client);
    return true;
}
bool ClientResult::exists(const string &username) const
{
    return Vector::exist(_data, &Client::getUsername, username);
}

ChannelResult ClientResult::getChannels(Client *client)
{
    ClientsChannelsResult clientsChannels = Db::clientsChannels.where(&ClientChannelMapping::getClientUsername, client->getUsername());
    return clientsChannels.getChannels();
}

template <typename TPropValue>
ClientResult ClientResult::where(const TPropValue &(Client::*getter)() const, const TPropValue &value)
{
    vector<Client *> res = Vector::where(_data, getter, value);
    return ClientResult(res);
}

template <typename TPropValue>
ClientResult ClientResult::where(TPropValue (Client::*getter)() const, TPropValue value)
{
    vector<Client *> res = Vector::where(_data, getter, value);
    return ClientResult(res);
}

ClientsChannelsResult ClientResult::getClientsChannels(Client *client)
{
    ClientsChannelsResult clientsChannels = Db::clientsChannels.where(&ClientChannelMapping::getClientUsername, client->getUsername());
    return clientsChannels;
}
