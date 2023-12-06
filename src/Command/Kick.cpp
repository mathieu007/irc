#include "Kick.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Channel.hpp"

void Kick::setVariableToZero()
{
	_errorMessage = "";
	_kickReasson = "";
	_channelName = "";
	_clientNickToKick = "";
	_clientToKick = nullptr;
}

std::string Kick::createReasonMessage(std::vector<std::string> tokens)
{
	std::string message;

	for (std::size_t i = 2; i < tokens.size(); ++i)
	{
		message += tokens[i];
		if (i < tokens.size() - 1)
		{
			message += " ";
		}
	}
	message = message.substr(1);
	std::cout << "message [" << message << "]" << std::endl;
	return message;
}

std::string Kick::createMessageToClient(Client *client, std::vector<std::string> tokens) {
	std::string message = ":" + client->getNickname() + " KICK " + _channelName + " " + tokens[2];
	if (tokens.size() == 4)
		message += " " + _kickReasson;
	return message + "\r\n";
}

bool Kick::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server)
{
	if (tokens.size() < 3 || tokens.size() > 4)
		_errorMessage = "461 " + client->getNickname() + " KICK :Not enought or too much parameters\r\n";
	else if (!server.channelExist(_channelName))
		_errorMessage = "403 " + client->getNickname() + " " + _channelName + " No such channel\r\n";
	else if (!server.isModerator(client, _channelName))
		_errorMessage = "482 " + client->getNickname() + " " + _channelName + " :You're not channel operator\r\n";
	else if (!server.isInChannel(_clientToKick, _channelName))
		_errorMessage = "441 " + client->getNickname() + " " + _channelName + _clientNickToKick + " :They aren't on that channel\r\n";
	else if (!server.isInChannel(client, _channelName))
		_errorMessage = "442 " + client->getNickname() + " " + _channelName + " :You're not on that channel\r\n";
	return _errorMessage.empty() ? true : false;
}

bool Kick::execute(Client *client, std::vector<std::string> tokens, Server &server)
{
	setVariableToZero();
	if (tokens.size() > 1)
		_channelName = tokens[1];
	if (tokens.size() > 2)
	{
		_clientNickToKick = tokens[2];
		_clientToKick = server.getClientByNickname(_clientNickToKick);
	}
	if (tokens.size() > 3)
		_kickReasson = createReasonMessage(tokens);

	if (!isValidCommand(tokens, client, server))
	{
		std::cout << RED << "Error sent to client: " << _errorMessage << RESET << std::endl;
		Msg::sendMsg(client, _errorMessage, 0);
	}
	else
	{
		std::cout << GREEN << "Executing KICK command" << RESET << std::endl;
		std::string messageToClient = createMessageToClient(client, tokens);
		std::cout << YELLOW << "Message sent to client: " << messageToClient << RESET << std::endl;
		Msg::sendMsg(client, messageToClient, 0);
		server.kickClientFromChannel(_clientToKick, _channelName);
	}

	return _errorMessage.empty() ? true : false;
}

Kick::~Kick() {}
