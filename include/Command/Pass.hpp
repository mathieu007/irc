#pragma once
#include "Command.hpp"
#include "Client.hpp"

class Pass : public Command
{
private:
	std::string _errorMessage;
	std::string _password;

	bool isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server);
	void setVariableToZero();

public:
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
	~Pass();
};