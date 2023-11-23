#include "Join.hpp"
#include "Server.hpp"
#include "Message.hpp"


bool Join::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server){
	_errorMessage = "";

	if (tokens.size() < 2 || tokens.size() > 3)
		_errorMessage = "461 JOIN :Not enought or too much parameters\r\n";
	if (tokens[1][0] != '#')
		_errorMessage = "476 " + tokens[1] + " :Bad Channel Mask\r\n";
	return _errorMessage.empty() ? true : false;
}

bool Join::execute(Client *client, std::vector<std::string> tokens, Server &server) {
	std::string channelName = tokens[1];

	if (!isValidCommand(tokens, client, server)) {
		std::cout << RED << "Error sent to client: " << _errorMessage << RESET << std::endl;
		sendMsg(client, _errorMessage, 0);
	}
	else {
		std::cout << GREEN << "Executing JOIN command" << RESET << std::endl;
		std::string messageToClient = ":" + client->getNickname() + " JOIN " + channelName + "\r\n";
		std::cout << YELLOW << "Message sent to client: " << messageToClient << RESET << std::endl;
		sendMsg(client, messageToClient, 0);
		server.join(client, channelName);
	}
	return _errorMessage.empty() ? true : false;
}

Join::~Join() {}

///reviser isvalid