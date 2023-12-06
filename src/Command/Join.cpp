#include "Join.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Channel.hpp"

void Join::setVariableToZero()
{
	_errorMessage = "";
	_channelName = "";
	_channelKey = "";
	_hasKey = 0;
}

bool Join::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server)
{
	Channel *channel = server.getChannel(_channelName);

	if (tokens.size() < 2 || tokens.size() > 3)
		_errorMessage = "461 " + client->getNickname() + " JOIN :Not enought or too much parameters\r\n";
	else if (_channelName.length() > 0 && _channelName[0] != '#')
		_errorMessage = "476 " + client->getNickname() + " " + _channelName + " :Bad Channel Mask\r\n";
	else if (client->isInChannel(channel))
		_errorMessage = "443 " + client->getNickname() + " " + _channelName + " :is already on channel\r\n";
	else if (server.isChannelFull(_channelName))
		_errorMessage = "471 " + client->getNickname() + " " + _channelName + " :Cannot join channel (+l)\r\n";
	else if (channel && channel->getKey() != _channelKey)
		_errorMessage = "475 " + client->getNickname() + " " + _channelName + " :Bad Channel Key\r\n";
	else if (client->isKickedFromChannel(channel))
		_errorMessage = "474 " + client->getNickname() + " " + _channelName + " :Cannot join channel (+b)\r\n";
	else if (channel && !channel->isModerator(client) && channel->isOnInvitationOnly() && !channel->isInIvitationList(client))
		_errorMessage = "473 " + client->getNickname() + " " + _channelName + " :Cannot join channel (+i)\r\n";
	return _errorMessage.empty() ? true : false;
}

bool Join::execute(Client *client, std::vector<std::string> tokens, Server &server)
{
	setVariableToZero();
	if (tokens.size() > 1)
		_channelName = tokens[1];
	// check again
	_hasKey = 0;
	if (tokens.size() == 3)
	{
		_channelKey = tokens[2];
		_hasKey = 1;
	}
	if (!isValidCommand(tokens, client, server))
	{
		std::cout << RED << "Error sent to client: " << _errorMessage << RESET << std::endl;
		Msg::sendMsg(client, _errorMessage, 0);
	}
	else
	{
		std::string messageToClient = ":" + client->getNickname() + " JOIN " + _channelName + "\r\n";
		std::cout << GREEN << "Executing JOIN command" << RESET << std::endl;
		std::cout << YELLOW << "Message sent to client: " << messageToClient << RESET << std::endl;
		// Msg::sendMsg(client, messageToClient, 0);
		if (_hasKey)
			server.join(client, _channelName, _channelKey);
		else
			server.join(client, _channelName);
		std::string userList;
		std::string messageToClientList;
		Channel *channel = server.getChannel(_channelName);
		std::vector<Client *> clients = server.getClientsInAChannel(channel);
		if (channel)
			channel->sendMsgToAll(messageToClient);
		for (std::vector<Client *>::size_type i = 0; i < clients.size(); ++i)
		{
			if (channel && channel->isModerator(clients[i]))
				userList += "@" + clients[i]->getNickname() + " ";
			else
				userList += clients[i]->getNickname() + " ";
		}

		for (std::vector<Client *>::size_type i = 0; i < clients.size(); ++i)
		{
			messageToClientList = "353 " + client->getNickname() + " = " + _channelName + " :" + userList + "\r\n";
			std::cout << YELLOW << "message sent to client:" << messageToClientList << RESET << std::endl;
			std::cout << "Client " << i << ": " << clients[i]->getNickname() << std::endl;
			Msg::sendMsgToRecipient(client, clients[i], messageToClientList, 0);
			messageToClientList = "366 " + client->getNickname() + " " + _channelName + " :End of /NAMES list\r\n";
			std::cout << YELLOW << "message sent to client:" << messageToClientList << RESET << std::endl;
			std::cout << "Client " << i << ": " << clients[i]->getNickname() << std::endl;
			Msg::sendMsgToRecipient(client, clients[i], messageToClientList, 0);
			std::cout << client->getNickname() << " " << clients[i]->getNickname() << std::endl;
		}

		// send channel topic
		if (server.hasTopic(_channelName))
		{
			std::string topic = server.getChannel(_channelName)->getTopic();
			messageToClient = "332 " + client->getNickname() + " " + _channelName + " :" + topic + "\r\n";
			std::cout << YELLOW << "Message sent to client: " << messageToClient << RESET << std::endl;
			Msg::sendMsg(client, messageToClient, 0);
		}
	}
	return _errorMessage.empty() ? true : false;
}

Join::~Join() {}
