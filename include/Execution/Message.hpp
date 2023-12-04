#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include "Client.hpp"
#include "Server.hpp"

class Server;
class Msg
{
public:
    static Server *_server;
    static ssize_t sendMsgToRecipient(Client *sender, Client *recipient, string &msg, int flags);
    static ssize_t sendMsg(Client *client, string &data, int flags);
    static string recvMsg(int sockfd, char *buffer);
    static bool parseAndExec(Client *client, string &msg, Server &server);
    static ssize_t sendQueuedMsg(Client *client, int flags);
    static bool sendAuthMessages(Client *client);
};
