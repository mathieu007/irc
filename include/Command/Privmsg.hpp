#pragma once
#include "Command.hpp"
#include "Client.hpp"

class Privmsg : public Command
{
private:
	std::string _errorMessage;

	std::string createMessage(std::vector<std::string> tokens);
	bool messageToChannel(Client *client, std::vector<std::string> tokens, Server &server);
	bool messageToClient(Client *client, std::vector<std::string> tokens, Server &server);
	bool isValidCommandToClient(std::vector<std::string> &tokens, Client *client, Server &server);
	bool isValidCommandToChannel(std::vector<std::string> &tokens, Client *client, Server &server);

public:
	~Privmsg();
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);

};
