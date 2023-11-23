#include "Privmsg.hpp"
#include "Server.hpp"
#include "Message.hpp"

bool Privmsg::isValidCommandToClient(std::vector<std::string> &tokens, Client *client, Server &server)
{
	_errorMessage = "";
	if (tokens.size() < 2)
		_errorMessage = "431 " + client->getHost() + " :No nickname given\r\n";
	return _errorMessage.empty() ? true : false;
}

bool Privmsg::isValidCommandToChannel(std::vector<std::string> &tokens, Client *client, Server &server)
{
	_errorMessage = "";
	if (tokens.size() < 2)
		_errorMessage = "431 " + client->getHost() + " :No nickname given\r\n";
	return _errorMessage.empty() ? true : false;
}

bool Privmsg::execute(Client *client, std::vector<std::string> tokens, Server &server)
{
	////msg to client
	if (tokens[1][0] != '#')
	{
		std::string senderNick = client->getNickname();
		std::string receiverNick = tokens[1];
		std::string message = tokens[2].substr(1);
		Client *receverClient = server.getClientByNickname(receiverNick);

		if (!isValidCommandToClient(tokens, client, server))
		{
			sendMsg(client, _errorMessage, 0);
			std::cout << "Error msg sent to client:" << RED << _errorMessage << RESET << std::endl;
		}
		else
		{
			std::cout << GREEN << "Executing PRIVMSG to user command" << RESET << std::endl;
			std::string messageToClient = ":" + senderNick + " Privmsg " + receiverNick + " " + message + "\r\n";
			std::cout << YELLOW << "message sent to client:" << messageToClient << RESET << std::endl;
			sendMsg(client, messageToClient, 0);
		}
	}
	////message to channel
	else
	{

		std::string senderNick = client->getNickname();
		std::string channelName = tokens[1];
		std::string message = tokens[2].substr(1);

		if (!isValidCommandToChannel(tokens, client, server))
		{
			sendMsg(client, _errorMessage, 0);
			std::cout << "Error msg sent to client:" << RED << _errorMessage << RESET << std::endl;
		}
		else
		{
			// std::cout << GREEN << "Executing PRIVMSG to channel command" << RESET << std::endl;
			// std::string messageToClient = ":" + senderNick + " PRIVMSG " + channelName + " " + message + "\r\n";
			// std::cout << YELLOW << "message sent to client:" << messageToClient << RESET << std::endl;
			// sendMsg(client, messageToClient, 0);
			Channel *channel = server.getChannel(channelName);
			std::vector<Client *> clients = server.getClientsInAChannel(channel);
			std::cout << "ici" << clients.size() << std::endl;

			for (std::vector<Client *>::size_type i = 0; i < clients.size(); ++i)
			{
				Client *currentClient = clients[i];
				std::cout << "Client " << i << ": " << currentClient->getNickname() << std::endl;

				std::string messageToClient = ":" + senderNick + " PRIVMSG " + channelName + " " + message + "\r\n";
				std::cout << YELLOW << "message sent to client:" << messageToClient << RESET << std::endl;
				sendMsg(currentClient, messageToClient, 0);
			}
		}
	}
	return _errorMessage.empty() ? true : false;
}

Privmsg::~Privmsg() {}