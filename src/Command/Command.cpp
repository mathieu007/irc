#include "Command.hpp"
#include "Server.hpp"

Command::Command() {}

Command::~Command() {}
bool Command::execute(Client *client, std::vector<std::string> tokens, Server &server)
{
    (void)client;
    (void)tokens;
    (void)server;
    return true;
}
