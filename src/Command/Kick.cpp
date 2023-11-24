#include "Kick.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Channel.hpp"

std::string Kick::createMessageToClient(Client *client, std::vector<std::string> tokens) {
	std::string message = ":" + client->getNickname() + " KICK " + _channelName + " " + client->getNickname();
	if (tokens.size() == 4)
		message += " " + _kickReasson;
	return message + "\r\n";
}

bool Kick::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server) {
	_errorMessage = "";

	if (tokens.size() < 3 || tokens.size() > 4)
		_errorMessage = "461 " + client->getHost() + " KICK :Not enought or too much parameters\r\n";
	else if (server.channelExist(_channelName))
		_errorMessage = "403 " +  client->getHost() + _channelName + " No such channel\r\n";
	else if (server.isModerator(client, _channelName))
		_errorMessage = "482 " + client->getHost() + _channelName + " :You're not channel operator\r\n";
	else if (server.isInChannel(_clientToKick, _channelName))
		_errorMessage = "441 " + client->getHost() + _channelName + _clientNickToKick + " :They aren't on that channel\r\n";
	else if (server.isInChannel(client, _channelName))
		_errorMessage = "442 " + client->getHost() + _channelName + " :You're not on that channel\r\n";
	return _errorMessage.empty() ? true : false;
}

bool Kick::execute(Client *client, std::vector<std::string> tokens, Server &server) {
	_channelName = tokens[1];
	_clientNickToKick = tokens[2];
	_kickReasson = tokens[3];
	_clientToKick = server.getClientByNickname(_clientNickToKick);

	if (!isValidCommand(tokens, client, server)) {
		std::cout << RED << "Error sent to client: " << _errorMessage << RESET << std::endl;
		sendMsg(client, _errorMessage, 0);
	}
	else {
		std::cout << GREEN << "Executing KICK command" << RESET << std::endl;
		std::string messageToClient = createMessageToClient(client, tokens);
		std::cout << YELLOW << "Message sent to client: " << messageToClient << RESET << std::endl;
		sendMsg(client, messageToClient, 0);
		server.kickClientFromChannel(_clientToKick, _channelName);
	}

	return _errorMessage.empty() ? true : false;
}

Kick::~Kick() {}

//reason to kick
