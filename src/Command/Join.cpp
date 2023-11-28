#include "Join.hpp"
#include "Server.hpp"
#include "Message.hpp"


bool Join::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server){
	_errorMessage = "";

	if (tokens.size() < 2 || tokens.size() > 3)
		_errorMessage = "461 " + client->getHost() + "JOIN :Not enought or too much parameters\r\n";
	if (_channelName[0] != '#')
		_errorMessage = "476 " + client->getHost() + _channelName + " :Bad Channel Mask\r\n";
		///////////rework/////////
	// if (hasKey) {
	// 	std::string key = tokens[3];
	// 	if (!server.channelExist(_channelName, key))
	// 		_errorMessage = "475 " + client->getHost() + _channelName + " :Cannot join channel (+k)\r\n";
	// }
	if (server.isInKickChannel(client, _channelName))
		_errorMessage = "474 " + client->getHost() + _channelName + " :Cannot join channel (+b)\r\n";
	return _errorMessage.empty() ? true : false;
}

bool Join::execute(Client *client, std::vector<std::string> tokens, Server &server) {
	_channelName = tokens[1];
	_hasKey = 0;
	if (tokens.size() == 3) {
		_channelKey = tokens[2];
		_hasKey = 1;
	}


	if (!isValidCommand(tokens, client, server)) {
		std::cout << RED << "Error sent to client: " << _errorMessage << RESET << std::endl;
		Msg::sendMsg(client, _errorMessage, 0);
	}
	else {
		std::cout << GREEN << "Executing JOIN command" << RESET << std::endl;
		std::string messageToClient = ":" + client->getNickname() + " JOIN " + _channelName + "\r\n";
		std::cout << YELLOW << "Message sent to client: " << messageToClient << RESET << std::endl;
		Msg::sendMsg(client, messageToClient, 0);
		if (_hasKey)
			server.join(client, _channelName, _channelKey);
		else
			server.join(client, _channelName);
	}
	return _errorMessage.empty() ? true : false;
}

Join::~Join() {}

///to many channel?
///to many in channel?
//topic
//473 invite;