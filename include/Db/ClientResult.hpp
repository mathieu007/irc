#pragma once

#include <string>
#include "ResultSet.hpp"

using std::string;
using std::vector;

class ClientsChannelsResult;
class Client;
class ChannelResult;

class ClientResult : public ResultSet<Client>
{
public:
    ClientResult();
    ClientResult(vector<Client *> &data);

    Client *getByUserName(const string &userName);
    Client *getByNickName(const string &nickName);
    
    bool removeByUserName(const string &userName);
    bool remove(Client *client);

    bool add(Client *client);
    bool exists(const string &username) const;

    template <typename TPropValue>
    ClientResult where(const TPropValue &(Client::*getter)() const, const TPropValue &value);
    template <typename TPropValue>
    ClientResult where(TPropValue (Client::*getter)() const, TPropValue value);

    ChannelResult getChannels(Client *client);
    ClientsChannelsResult getClientsChannels(Client *client);
};