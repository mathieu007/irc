#include "User.hpp"
#include "Server.hpp"
#include "Message.hpp"

void User::setVariableToZero(){
	_errorMessage = "";
	_newUserName = "";
	_newRealName = "";
}

bool User::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server) {
	if (tokens.size() < 5)
		_errorMessage = "461 " + client->getNickname() + " USER :Not enough parameters\r\n";
	if (tokens.size() > 5)
		_errorMessage = "1002 " + client->getNickname() + " USER :Too much parameters\r\n";
	else if (server.isAuthenticated(client))
		_errorMessage = "462 " + client->getNickname() + " :You may not reregister\r\n";
	else if (server.userNameExist(_newUserName))
		_errorMessage = "462 " + client->getNickname() + " :Username already exist\r\n";		
	return _errorMessage.empty() ? true : false;
}

bool User::execute(Client *client, std::vector<std::string> tokens, Server &server) {
	setVariableToZero();
	if (tokens.size() > 1)
 		_newUserName = tokens[1];
	if (tokens.size() > 4)
		_newRealName = tokens[4].substr(1);

	if (!isValidCommand(tokens, client, server)) {
		Msg::sendMsg(client, _errorMessage, 0);
		std::cout << "Error msg sent to client:" << RED << _errorMessage << RESET << std::endl;
	}
	else {
		std::cout << GREEN << "Executing USER command" << RESET << std::endl;
		client->setUsername(_newUserName);
		client->setRealname(_newRealName);
	}
	return _errorMessage.empty() ? true : false;
}

User::~User() {}

//cacatonate token 5+ into real name