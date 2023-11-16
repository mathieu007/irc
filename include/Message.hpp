#pragma once

#include "Server.hpp"

bool checkClient(Client *client, Server &server);
bool sendToClient(Client *client, string &msg);
size_t nonBlockingSend(Client *client, string &data, int flags);