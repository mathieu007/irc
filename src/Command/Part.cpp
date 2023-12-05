#include "Part.hpp"
#include "Server.hpp"
#include "Message.hpp"

void Part::setVariableToZero()
{
	_errorMessage = "";
	_channelName = "";
	_reasonToLeave = "";
}

std::string Part::createReasonMessage(std::vector<std::string> tokens) {
	std::string message;

	for (std::size_t i = 2; i < tokens.size(); ++i) {
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

bool Part::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server) {;
	(void)server;
	if (tokens.size() > 1)
		_channelName = tokens[1];
	if (tokens.size() > 2)
		_reasonToLeave = createReasonMessage(tokens);
	if (tokens.size() < 2)
		_errorMessage = "461 " + client->getHost() + " PART :Not enought or too much parameters\r\n";
	else if (!server.channelExist(_channelName))
		_errorMessage = "403 " + client->getNickname() + " " + _channelName + " No such channel\r\n";
	else if (!server.isInChannel(client, _channelName))
		_errorMessage = "442 " + client->getNickname() + " " + _channelName + " :You're not on that channel\r\n";
	return _errorMessage.empty() ? true : false;
}

std::string Part::createPartMessage(Client *client, const std::vector<std::string> &tokens) {
	return ":" + client->getNickname() + " PART " + tokens[1] + " " + _reasonToLeave + "\r\n";
}

bool Part::execute(Client *client, std::vector<std::string> tokens, Server &server)
{
	setVariableToZero();

	if (!isValidCommand(tokens, client, server)) {
		Msg::sendMsg(client, _errorMessage, 0);
		std::cout << RED << "Error sent to client:" << _errorMessage << RESET << std::endl;
	}
	std::cout << GREEN << "Executing PART command" << RESET << std::endl;
	std::string messageToClient = createPartMessage(client, tokens);
	std::cout << YELLOW << "Message sent to client: " << messageToClient << RESET << std::endl;
	Msg::sendMsg(client, messageToClient, 0);
	server.removeClientFromChannel(client, tokens[1]);
	// send user list for dave

		std::string userList;
		Channel *channel = server.getChannel(_channelName);
		std::vector<Client *> clients = server.getClientsInAChannel(channel);
		std::string messageToClientList = ":" + client->getNickname() + " PART " + tokens[1] + " " + _reasonToLeave + "\r\n";

		for (std::vector<Client *>::size_type i = 0; i < clients.size(); ++i){
			Msg::sendMsgToRecipient(client, clients[i], messageToClientList, 0);
		}
	return _errorMessage.empty() ? true : false;
}

Part::~Part() {}
