#pragma once
#include "Client.hpp"
#include "Command.hpp"
#include "Message.hpp"

class Nick : public Command
{
private:
	std::string _errorMessage;

public:
	bool execute(Client *client, std::vector<std::string> tokens, IServer &server);
	bool isValidCommand(std::vector<std::string> &tokens, Client *client, IServer &server);
};