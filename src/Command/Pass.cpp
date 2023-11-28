#include "Pass.hpp"
#include "Server.hpp"
#include "Message.hpp"

void Pass::setVariableToZero(){
	_errorMessage = "";
	_password = "";
}

bool Pass::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server)
{
	(void)server;
	if (tokens.size() < 2)
		_errorMessage = "461 " + client->getHost() + " PASS :Not enough parameters\r\n";
	else if (tokens.size() > 2)
		_errorMessage = "1002 " + client->getHost() + " PASS :Too much parameters\r\n";
	if (server.isAuthenticated(client))
		_errorMessage = "462 " + client->getHost() + " :You may not reregister\r\n";
	else if (!server.isValidPassword(client))
		_errorMessage = "464 " + client->getHost() + " :Password incorrect\r\n";
	return _errorMessage.empty() ? true : false;
}

bool Pass::execute(Client *client, std::vector<std::string> tokens, Server &server)
{
	setVariableToZero();
	if (tokens.size() > 1)
		_password = tokens[1];
	server.setClientPassword(client, _password);
	if (!isValidCommand(tokens, client, server)) {
		Msg::sendMsg(client, _errorMessage, 0);
		std::cout << "Error msg sent to client:" << RED << _errorMessage << RESET << std::endl;
		_password = "";
	}
	else {
		std::cout << GREEN << "Executing PASS command" << RESET << std::endl;
	}
	return _errorMessage.empty() ? true : false;
}

Pass::~Pass() {}