#pragma once
#include "Command.hpp"
#include "Client.hpp"

class Privmsg : public Command
{
private:
	std::string _errorMessage;

public:
	~Privmsg();
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
	bool isValidCommandToClient(std::vector<std::string> &tokens, Client *client, Server &server);
	bool isValidCommandToChannel(std::vector<std::string> &tokens, Client *client, Server &server);

};
