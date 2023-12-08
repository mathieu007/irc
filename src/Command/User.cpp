#include "User.hpp"
#include "Server.hpp"
#include "Message.hpp"

void User::setVariableToZero()
{
	_errorMessage = "";
	_newUserName = "";
	_newRealName = "";
}

std::string User::createFullName(std::vector<std::string> tokens)
{
	std::string message;

	for (std::size_t i = 4; i < tokens.size(); ++i)
	{
		message += tokens[i];
		if (i < tokens.size() - 1)
		{
			message += " ";
		}
	}
	// message = message.substr(4);
	std::cout << "message [" << message << "]" << std::endl;
	return message;
}

bool User::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server)
{
	if (tokens.size() < 5)
		_errorMessage = "461 " + client->getNickname() + " USER :Not enough parameters\r\n";
	else if (server.isAuthenticated(client))
		_errorMessage = "462 " + client->getNickname() + " :You may not reregister\r\n";
	else if (server.userNameExist(_newUserName))
		_errorMessage = "462 " + client->getNickname() + " :Username already exist\r\n";
	return _errorMessage.empty() ? true : false;
}

bool User::execute(Client *client, std::vector<std::string> tokens, Server &server)
{
	setVariableToZero();
	static int id = 0;

	if (tokens.size() > 1)
		_newUserName = tokens[1];
	if (tokens.size() > 4)
		_newRealName = createFullName(tokens);
	if (server.userNameExist(_newUserName))
	{
		id++;
		std::stringstream ss;
		ss << id;
		std::string myString = ss.str();
		string oldUsername = _newUserName;
		_newUserName = _newUserName + myString;
		// std::string messageToClient = ":" + oldUsername + " NOTICE :Your Username have been changed to: " + _newUserName + "\r\n";
		// Msg::sendMsg(client, messageToClient, 0);
	}
	if (!isValidCommand(tokens, client, server))
	{
		Msg::sendMsg(client, _errorMessage, 0);
		std::cout << "Error msg sent to client:" << RED << _errorMessage << RESET << std::endl;
	}
	else
	{
		client->setIsAuth(true);
		std::cout << GREEN << "Executing USER command" << RESET << std::endl;
		client->setUsername(_newUserName);
		client->setRealname(_newRealName);
	}
	return _errorMessage.empty() ? true : false;
}

User::~User() {}
