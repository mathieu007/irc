#pragma once
#include "Server.hpp"

class Message
{
public:
    bool checkClient(Client *client, Server &server);
    bool sendToClient(Client *client, string &msg);
};