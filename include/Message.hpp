#pragma once

#include "Server.hpp"

ssize_t nonBlockingSend(Client *client, string &data, int flags);
// std::vector<std::string> tokenMessage(std::string message);