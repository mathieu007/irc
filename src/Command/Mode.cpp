#include "Mode.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Channel.hpp"

void Mode::setVariableToZero()
{
	_errorMessage = "";
	_channelName = "";
}

bool Mode::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server)
{
	(void)server;
	if (tokens.size() < 3)
		_errorMessage = "461 " + client->getNickname() + " MODE :Not enough parameters\r\n";

	return _errorMessage.empty() ? true : false;
}

void Mode::setInviteOnly(std::vector<std::string> &tokens, Client *client, Server &server)
{
	std::string channelName = tokens[1];
	Channel *channel = server.getChannel(channelName);

	if (tokens.size() != 3)
		_errorMessage = "461 " + client->getNickname() + " " + " MODE :Bad number of parameters\r\n";
	else if (tokens[2][0] != '-' && tokens[2][0] != '+')
		_errorMessage = "501 " + client->getNickname() + " " + " MODE " + tokens[2] + " :Unknown MODE flag\r\n";
	else if (!server.channelExist(channelName))
		_errorMessage = "403 " + client->getNickname() + " " + channelName + " :No such channel\r\n";
	else if (!server.isModerator(client, channelName))
		_errorMessage = "482 " + client->getNickname() + " " + channelName + " :You're not channel operator\r\n";
	else if (tokens[2][0] == '+' && channel->isOnInvitationOnly())
		_errorMessage = "666 " + client->getNickname() + " " + channelName + " :Is already on invite only\r\n";
	else if (tokens[2][0] == '-' && !channel->isOnInvitationOnly())
		_errorMessage = "666 " + client->getNickname() + " " + channelName + " :Is already not on invite only\r\n";
	if (_errorMessage.empty())
	{
		std::cout << GREEN << "Executing MODE command" << RESET << std::endl;
		if (tokens[2][0] == '-')
			channel->setJoinOnInvitationOnly(0);
		else if (tokens[2][0] == '+')
			channel->setJoinOnInvitationOnly(1);
	}
	else
	{
		Msg::sendMsg(client, _errorMessage, 0);
		std::cout << RED << "Error msg sent to client:" << _errorMessage << RESET << std::endl;
	}
}

void Mode::setTopicByOperatorOnly(std::vector<std::string> &tokens, Client *client, Server &server)
{
	std::string channelName = tokens[1];
	Channel *channel = server.getChannel(channelName);
	(void)channel;

	if (tokens.size() != 3)
		_errorMessage = "461 " + client->getNickname() + " MODE :Bad number of parameters\r\n";
	else if (tokens[2][0] != '-' && tokens[2][0] != '+')
		_errorMessage = "501 " + client->getNickname() + " MODE " + tokens[2] + " :Unknown MODE flag\r\n";
	else if (!server.isModerator(client, channelName))
		_errorMessage = "482 " + client->getNickname() + " " + channelName + " :You're not channel operator\r\n";
	else if (!server.channelExist(channelName))
		_errorMessage = "403 " + client->getNickname() + " " + channelName + " :No such channel\r\n";
	// else if (tokens[2][0] == '+' && channel->topicsetablebyoperator() )
	// 	_errorMessage = "666 " + client->getHost() + _channelName + " :Is already settable only by operator\r\n";
	// else if (tokens[2][0] == '-' && !channel->topicsetablebyoperator())
	// 	_errorMessage = "666 " + client->getHost() + _channelName + " :Is already settable not only by operator\r\n";
	// if (_errorMessage.empty()) {
	// 	std::cout << GREEN << "Executing MODE command" << RESET << std::endl;
	// 	if (tokens[2][0] == '-')
	// 		channel->settopmode(0);
	// 	else if (tokens[2][0] == '+')
	// 		channel->setJoinOnInvitationOnly(0);
	// }
	// else {
	// 	Msg::sendMsg(client, _errorMessage, 0);
	// 	std::cout << RED << "Error msg sent to client:"  << _errorMessage << RESET << std::endl;
	// }
}

bool Mode::execute(Client *client, std::vector<std::string> tokens, Server &server)
{
	setVariableToZero();

	if (tokens.size() > 1)
	{
		if (tokens[2].size() == 2 && tokens[2][1] == 'i')
			setInviteOnly(tokens, client, server);
		if (tokens[2].size() == 2 && tokens[2][1] == 't')
			setTopicByOperatorOnly(tokens, client, server);
	}

	// if (!isValidCommand(tokens, client, server)) {
	// 	Msg::sendMsg(client, _errorMessage, 0);
	// 	std::cout << RED << "Error msg sent to client:"  << _errorMessage << RESET << std::endl;
	// }
	// else {
	// 	std::cout << GREEN << "Executing MODE command" << RESET << std::endl;
	// }
	return _errorMessage.empty() ? true : false;
}

Mode::~Mode() {}