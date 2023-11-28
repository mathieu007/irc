#include "User.hpp"
#include "Server.hpp"
#include "Message.hpp"

bool User::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server)
{
	(void)server;
	_errorMessage = "";
	if (tokens.size() < 5)
		_errorMessage = "461 " + client->getHost() + " USER :Not enough parameters\r\n";
	if (tokens.size() > 5)
		_errorMessage = "1002 " + client->getHost() + " USER :Too much parameters\r\n";
	else if (server.isAuthenticated(client))
		_errorMessage = "462 " + client->getHost() + " :You may not reregister\r\n";
	return _errorMessage.empty() ? true : false;
}

bool User::execute(Client *client, std::vector<std::string> tokens, Server &server)
{
	(void)server;

	if (!isValidCommand(tokens, client, server))
	{
		Msg::sendMsg(client, _errorMessage, 0);
		std::cout << "Error msg sent to client:" << RED << _errorMessage << RESET << std::endl;
	}
	else
	{
		std::cout << GREEN << "Executing USER command" << RESET << std::endl;
		std::string newUserName = tokens[1];
		std::string newRealName = tokens[4].substr(1);
		client->setUsername(newUserName);
		client->setRealname(newRealName);
	}
	return _errorMessage.empty() ? true : false;
}

User::~User() {}