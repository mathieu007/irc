#pragma once
#include "Command.hpp"

class Part : public Command
{
private:
	std::string _errorMessage;

public:
	~Part();
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
	std::string createPartMessage(Client *client, const std::vector<std::string> &tokens);
};