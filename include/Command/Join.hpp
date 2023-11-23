#pragma once
#include "Command.hpp"

class Join : public Command
{
private:
	std::string _errorMessage;
	bool isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server);

public:
	~Join();
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
};