#include "Pass.hpp"
#include "Server.hpp"
#include "Message.hpp"

bool Pass::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server){
	std::string password = tokens[1];
	_errorMessage = "";
	if (tokens.size() < 2)
		_errorMessage = "461 " + client->getHost() + " PASS :Not enough parameters\r\n";
	else if (tokens.size() > 2)
		_errorMessage = "1002 " + client->getHost() + " PASS :Too much parameters\r\n";
	else if (client->isAuthorized())
		_errorMessage = "462 " + client->getHost() + " :You may not reregister\r\n";
	return _errorMessage.empty() ? true : false;
}

bool Pass::execute(Client *client, std::vector<std::string> tokens, Server &server){
	std::string password = tokens[1];
	if (!isValidCommand(tokens, client, server)) {
		sendMsg(client, _errorMessage, 0);
		std::cout << "Error msg sent to client:" << RED << _errorMessage << RESET << std::endl;
	}
	else {
		std::cout << GREEN << "Executing PASS command" << RESET << std::endl;
		client->setPass(password);
	}
	return _errorMessage.empty() ? true : false;
}

Pass::~Pass(){}