#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include "Client.hpp"
#include "Server.hpp"

ssize_t sendMsg(Client *client, string &data, int flags);
string recvMsg(int sockfd, char *buffer, int flags);
bool parseAndExec(Client *client, string &msg, Server &server);
