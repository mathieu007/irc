#include "Privmsg.hpp"
#include "Server.hpp"
#include "Message.hpp"

std::string Privmsg::createMessage(std::vector<std::string> tokens) {
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

bool Privmsg::isValidCommandToClient(std::vector<std::string> &tokens, Client *client, Server &server) {
	(void)server;
	_errorMessage = "";
	if (tokens.size() < 2)
		_errorMessage = "431 " + client->getHost() + " :No nickname given\r\n";

	return _errorMessage.empty() ? true : false;
}

bool Privmsg::messageToClient(Client *client, std::vector<std::string> tokens, Server &server)
{
	std::string receiverNick = "";

	std::string senderNick = client->getNickname();
	if (tokens.size() > 1)
		receiverNick = tokens[1];
	std::string message = createMessage(tokens);
	// Client *receverClient = server.getClientByNickname(receiverNick);

	if (!isValidCommandToClient(tokens, client, server)){
		Msg::sendMsg(client, _errorMessage, 0);
		std::cout << "Error msg sent to client:" << RED << _errorMessage << RESET << std::endl;
	}
	else
	{
		Client *recipient = server.getClientByNickname(receiverNick);
		std::cout << GREEN << "Executing PRIVMSG to user command" << RESET << std::endl;
		std::string messageToClient = ":" + senderNick + " Privmsg " + receiverNick + " " + message + "\r\n";
		std::cout << YELLOW << "message sent to client:" << messageToClient << RESET << std::endl;
		Msg::sendMsgToRecipient(client, recipient, messageToClient, 0);
	}
	return _errorMessage.empty() ? true : false;
}

bool Privmsg::isValidCommandToChannel(std::vector<std::string> &tokens, Client *client, Server &server)
{
	(void)server;
	_errorMessage = "";
	if (tokens.size() < 2)
		_errorMessage = "431 " + client->getHost() + " :No nickname given\r\n";
	return _errorMessage.empty() ? true : false;
}

bool Privmsg::messageToChannel(Client *client, std::vector<std::string> tokens, Server &server) {
	std::string channelName = "";

	std::string senderNick = client->getNickname();
	if (tokens.size() > 1)
		channelName = tokens[1];
	std::string message = createMessage(tokens);

	if (!isValidCommandToChannel(tokens, client, server))
	{
		Msg::sendMsg(client, _errorMessage, 0);
		std::cout << "Error msg sent to client:" << RED << _errorMessage << RESET << std::endl;
	}
	else
	{
		Channel *channel = server.getChannel(channelName);
		std::vector<Client *> clients = server.getClientsInAChannel(channel);
		std::string messageToClient;
		for (std::vector<Client *>::size_type i = 0; i < clients.size(); ++i)
		{
			Client *recipient = clients[i];
			if (client != recipient)
			{
				messageToClient = ":" + senderNick + " PRIVMSG " + channelName + " :" + message + "\r\n";
				std::cout << YELLOW << "message sent to client:" << messageToClient << RESET << std::endl;
				std::cout << "Client " << i << ": " << recipient->getNickname() << std::endl;
				Msg::sendMsgToRecipient(client, recipient, messageToClient, 0);
			}
		}
	}
	return _errorMessage.empty() ? true : false;
}

bool Privmsg::execute(Client *client, std::vector<std::string> tokens, Server &server)
{
	if (tokens[1][0] != '#')
		messageToClient(client, tokens, server);
	else
		messageToChannel(client, tokens, server);
	return _errorMessage.empty() ? true : false;
}

Privmsg::~Privmsg() {}