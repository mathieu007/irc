#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include "Client.hpp"
#include "CommandFactory.hpp"

class Server;

ssize_t nonBlockingSend(Client *client, string &data, int flags);
string nonBlockingRecv(int sockfd, char *buffer, int flags);
bool parseExec(Client *client, string &msg, Server &server);
