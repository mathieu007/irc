#pragma once
#include "Command.hpp"
#include "Client.hpp"

class Pass : public Command
{
private:
	std::string _errorMessage;

public:
	~Pass();
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
	bool isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server);
};