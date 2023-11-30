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
	else if (server.isChannelFull(_channelName))
		_errorMessage = "471 " + client->getNickname() + " " + _channelName + " :Cannot join channel (+l)\r\n";
	else if (_channelKey != "" && server.channelExist(_channelName) && !server.channelKeyExist(_channelName, _channelKey))
		_errorMessage = "475 " + client->getNickname() + " " + _channelName + " :Bad Channel Key\r\n";
	else if (server.isKickedFromChannel(client, _channelName))
		_errorMessage = "474 " + client->getNickname() + " " + _channelName + " :Cannot join channel (+b)\r\n";
	else if (channel && channel->isOnInvitationOnly() && !channel->isAllowedToJoin(client))
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
		std::cout << GREEN << "Executing JOIN command" << RESET << std::endl;
		std::string messageToClient = ":" + client->getNickname() + " JOIN " + _channelName + "\r\n";
		std::cout << YELLOW << "Message sent to client: " << messageToClient << RESET << std::endl;
		Msg::sendMsg(client, messageToClient, 0);
		if (_hasKey)
			server.join(client, _channelName, _channelKey);
		else
			server.join(client, _channelName);
		// send channel topic
		if (server.hasTopic(_channelName))
		{
			std::string topic = server.getChannel(_channelName)->getTopic();
			messageToClient = "332 " + client->getNickname() + " " + _channelName + " :" + topic + "\r\n";
			std::cout << YELLOW << "Message sent to client: " << messageToClient << RESET << std::endl;
			Msg::sendMsg(client, messageToClient, 0);
		}
		// else
		// 	messageToClient = client->getHost() + " " + _channelName + " :No topic is set\r\n";
	}
	return _errorMessage.empty() ? true : false;
}

Join::~Join() {}

/// to many channel?
/// to many in channel?