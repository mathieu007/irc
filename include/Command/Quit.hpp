#pragma once
#include "Command.hpp"

class Quit : public Command
{
private:
	std::string _errorMessage;

public:
	~Quit();
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
	std::string createQuitMessage(Client *client, const std::vector<std::string> &tokens);
};