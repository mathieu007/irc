#include "Message.hpp"

bool Message::checkClient(Client *client, Server &server)
{
    if (client.getPass() != server.getPass())
    {
        sendToClient(client, ":irc You didn't registered a Pass to connect to the server. (use /PASS)\r\n");
        return (false);
    }
    else if (client.getUsername().empty())
    {
        sendToClient(client, ":irc You didn't registered a Username. (use /USER)\r\n");
        return (false);
    }
    else if (client.getNickname().empty())
    {
        sendToClient(client, ":irc You didn't registered a Nickname. (use /NICK)\r\n");
        return (false);
    }
    return (true);
}

bool Message::sendToClient(Client *client, string &msg)
{
    // send here.
    return (true);
}