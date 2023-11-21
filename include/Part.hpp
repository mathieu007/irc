#pragma once
#include "Client.hpp"
#include "Command.hpp"
#include "Message.hpp"
#include "Channel.hpp"

class Part : public Command
{
private:
	std::string _errorMessage;

public:
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
	std::string createPartMessage(Client *client, const std::vector<std::string> &tokens);
};