#include "Topic.hpp"
#include "Server.hpp"
#include "Message.hpp"

void Topic::setVariableToZero()
{
	_errorMessage = "";
	_channelTopic = "";
	_channelName = "";
	_channel = nullptr;
}

std::string Topic::createTopicMessage(std::vector<std::string> tokens)
{
	std::string message;

	for (std::size_t i = 2; i < tokens.size(); ++i)
	{
		message += tokens[i];
		if (i < tokens.size() - 1)
		{
			message += " ";
		}
	}
	// message = message.substr(1);
	std::cout << "message [" << message << "]" << std::endl;
	return message;
}

bool Topic::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server)
{
	_errorMessage = "";

	Channel *channel = server.getChannel(_channelName);

	if (tokens.size() < 3)
		_errorMessage = "461 " + client->getNickname() + " TOPIC :Not enought parameters\r\n";
	else if (!channel)
		_errorMessage = "403 " + client->getNickname() + " " + _channelName + " :No such channel\r\n";
	else if (!server.isInChannel(client, _channelName))
		_errorMessage = "442 " + client->getNickname() + " " + _channelName + " :You're not on that channel\r\n";
	else if (channel && !channel->isTopicPublic() && !server.isModerator(client, _channelName))
		_errorMessage = "482 " + client->getNickname() + " " + _channelName + " :You're not channel operator\r\n";
	return _errorMessage.empty() ? true : false;
}

bool Topic::execute(Client *client, std::vector<std::string> tokens, Server &server)
{
	setVariableToZero();
	if (tokens.size() > 1)
		_channelName = tokens[1];
	if (tokens.size() > 2)
		_channelTopic = createTopicMessage(tokens);
	if (!isValidCommand(tokens, client, server))
	{
		Msg::sendMsg(client, _errorMessage, 0);
		std::cout << "Error msg sent to client:" << RED << _errorMessage << RESET << std::endl;
	}
	else
	{
		std::cout << GREEN << "Executing TOPIC command :" << _channelTopic << RESET << std::endl;
		_channel = server.getChannel(_channelName);
		_channel->setTopic(_channelTopic);
		string msg = ":" + client->getNickname() + " TOPIC " + _channelName + " :" + _channelTopic + "\r\n";
		_channel->sendMsgToAll(msg);
		std::cout << "TOPIC msg sent to clients:" << RED << msg << RESET << std::endl;
	}
	return _errorMessage.empty() ? true : false;
}

Topic::~Topic() {}
