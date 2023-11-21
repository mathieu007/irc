#pragma once
#include "Client.hpp"
#include "Command.hpp"
#include "Message.hpp"

class Nick : public Command
{
private:
	// std::string _NickMessageToClient;

public:
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
};