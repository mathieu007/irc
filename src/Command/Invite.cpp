#include "Invite.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Client.hpp"

void Invite::setVariableToZero(){
	_errorMessage = "";
	_channelName = "";
	_clientNickToInvite = "";
	_clientToInvite = nullptr;
}

bool Invite::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server) {
	if (tokens.size() != 3)
		_errorMessage = "461 " + client->getNickname() + " KICK :Not enought or too much parameters\r\n";
	else if (!server.channelExist(_channelName))
		_errorMessage = "403 " + client->getNickname() + " " + _channelName + " No such channel\r\n";
	else if (!server.isModerator(client, _channelName))
		_errorMessage = "482 " + client->getNickname() + " " + _channelName + " :You're not channel operator\r\n";
	else if (server.isInChannel(_clientToInvite, _channelName))
		_errorMessage = "441 " + client->getNickname() + " " + _channelName + _clientNickToInvite + " :They already on that channel\r\n";
	else if (!server.isInChannel(client, _channelName))
		_errorMessage = "442 " + client->getNickname() + " " + _channelName + " :You're not on that channel\r\n";
	return _errorMessage.empty() ? true : false;
}

bool Invite::execute(Client *client, std::vector<std::string> tokens, Server &server) {
	setVariableToZero();
	if (tokens.size() > 1)
		_channelName = tokens[1];
	if (tokens.size() > 2) {
		_clientNickToInvite = tokens[2];
		_clientToInvite = server.getClientByNickname(_clientNickToInvite);
	}
	if (!isValidCommand(tokens, client, server)) {
		std::cout << RED << "Error sent to client: " << _errorMessage << RESET << std::endl;
		Msg::sendMsg(client, _errorMessage, 0);
	}
	else {
		std::cout << GREEN << "Executing Invite command" << RESET << std::endl;
		std::string messageToClient = createMessageToClient(client, tokens);
		std::cout << YELLOW << "Message sent to client: " << messageToClient << RESET << std::endl;
		Msg::sendMsg(client, messageToClient, 0);
		// server.lacomandekilajob();
	}

	return _errorMessage.empty() ? true : false;
}

Invite::~Invite() {}