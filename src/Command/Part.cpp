#include "Part.hpp"
#include "Server.hpp"
#include "Message.hpp"

bool Part::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server) {
	_errorMessage = "";
	// if (tokens.size() != 2)
	// 	_errorMessage = "461 " + client->getHost() + " KICK :Not enought or too much parameters\r\n";
	return _errorMessage.empty() ? true : false;
}

std::string Part::createPartMessage(Client *client, const std::vector<std::string> &tokens) {
	return ":" + client->getNickname() + " PART " + tokens[1] + " " + tokens[2] + "\r\n";
}

bool Part::execute(Client *client, std::vector<std::string> tokens, Server &server) {
	if (!isValidCommand(tokens, client, server)) {
		sendMsg(client, _errorMessage, 0);
		std::cout << RED << "Error sent to client:" << _errorMessage << RESET << std::endl;
	}
	std::cout << GREEN << "Executing PART command" << RESET << std::endl;
	std::string messageToClient = createPartMessage(client, tokens);
	std::cout << YELLOW << "Message sent to client: " << messageToClient << RESET << std::endl;
	sendMsg(client, messageToClient, 0);
	server.removeClientFromChannel(client, tokens[1]);
	return _errorMessage.empty() ? true : false;
}

Part::~Part() {}

//leaving reason parsing