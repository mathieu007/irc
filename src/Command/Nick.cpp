#include "Nick.hpp"
#include "Server.hpp"
#include "Message.hpp"

void Nick::setVariableToZero()
{
	_errorMessage = "";
	_newNickName = "";
	_oldNickName = "";
}

bool Nick::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server)
{
	if (tokens.size() < 2)
		_errorMessage = "431 " + client->getNickname() + " :No nickname given\r\n";
	else if (tokens[1].size() > 20)
		_errorMessage = "1001 " + client->getNickname() + " :Nickname too long\r\n";
	else if (_newNickName.length() > 0 && (isdigit(_newNickName[0]) || _newNickName[0] == '#' || _newNickName[0] == ':' || _newNickName.find(" ") != std::string::npos))
		_errorMessage = "432 " + client->getNickname() + " " + _newNickName + " :Erroneus nickname\r\n";
	else if (server.nickNameExist(_newNickName))
		_errorMessage = "433 " + client->getNickname() + " " + _newNickName + " :Nickname is already in use\r\n";
	return _errorMessage.empty() ? true : false;
}

bool Nick::execute(Client *client, std::vector<std::string> tokens, Server &server)
{
	setVariableToZero();
	if (tokens.size() > 1)
		_newNickName = tokens[1];
	_oldNickName = client->getNickname();
	if (_oldNickName == "")
		_oldNickName = "backupnicknameforspe";

	if (!isValidCommand(tokens, client, server))
	{
		Msg::sendMsg(client, _errorMessage, 0);
		std::cout << "Error msg sent to client:" << RED << _errorMessage << RESET << std::endl;
	}
	else
	{
		std::cout << GREEN << "Executing NICK command" << RESET << std::endl;
		client->setNickname(_newNickName);
		std::string messageToClient = ":" + _oldNickName + " NICK :" + _newNickName + "\r\n";
		std::cout << YELLOW << "message sent to client:" << messageToClient << RESET << std::endl;
		Msg::sendMsg(client, messageToClient, 0);
	}
	return _errorMessage.empty() ? true : false;
}

Nick::~Nick() {}