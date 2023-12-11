#include "Invite.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Client.hpp"

void Invite::setVariableToZero()
{
	_errorMessage = "";
	_channelName = "";
	_clientNickToInvite = "";
	_clientToInvite = nullptr;
}

bool Invite::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server)
{
	if (tokens.size() > 2)
	{
		_channelName = tokens[2];
		_clientNickToInvite = tokens[1];
		_clientToInvite = server.getClientByNickname(_clientNickToInvite);
		_channel = server.getChannel(_channelName);
	}
	if (tokens.size() != 3)
		_errorMessage = "461 " + client->getNickname() + " KICK :Not enought or too much parameters\r\n";
	else if (!_channel)
		_errorMessage = "403 " + client->getNickname() + " " + _channelName + " :No such channel\r\n";
	else if (!_clientToInvite)
		_errorMessage = "401 " + client->getNickname() + " " + _clientNickToInvite + " :No such user\r\n";
	else if (!server.isModerator(client, _channelName))
		_errorMessage = "482 " + client->getNickname() + " " + _channelName + " :You're not channel operator\r\n";
	else if (_clientToInvite->isInChannel(_channel) && _channel->isInIvitationList(_clientToInvite))
		_errorMessage = "441 " + client->getNickname() + " " + _channelName + " " + _clientNickToInvite + " :is already on that channel\r\n";
	else if (_channel->isOnInvitationOnly() && _channel->isInIvitationList(_clientToInvite))
		_errorMessage = "443 " + client->getNickname() + " " + _clientNickToInvite + " " + _channelName + " :is already invited on that channel\r\n";
	else if (!_channel->isOnInvitationOnly())
		_errorMessage = "443 " + client->getNickname() + " " + _clientNickToInvite + " " + _channelName + " :cannot add to invitation list, channel is not in invite mode +i\r\n";
	else if (!client->isInChannel(_channel))
		_errorMessage = "442 " + client->getNickname() + " " + _channelName + " :You're not on that channel\r\n";
	return _errorMessage.empty() ? true : false;
}

bool Invite::execute(Client *client, std::vector<std::string> tokens, Server &server)
{
	setVariableToZero();

	if (!isValidCommand(tokens, client, server))
	{
		std::cout << RED << "Error sent to client: " << _errorMessage << RESET << std::endl;
		Msg::sendMsg(client, _errorMessage, 0);
		return false;
	}
	std::cout << GREEN << "Executing Invite command" << RESET << std::endl;
	// make it compile....
	//  channel
	if (_channel->isBanned(_clientToInvite))
	{
		string unbannedMessage = ":" + client->getNickname() + " MODE " + _channelName + " -b :You have been unbanned\r\n";
		Msg::sendMsg(_clientToInvite, unbannedMessage, 0);
	}
	_channel->addToInvitation(_clientToInvite);
	string rpl = "341 " + client->getNickname() + " " + _clientNickToInvite + " " + _channelName + "\r\n";
	Msg::sendMsg(client, rpl, 0);
	string invitemsg = ":" + client->getNickname() + " INVITE " + _clientNickToInvite + " " + _channelName + " :You've been invited\r\n";
	Msg::sendMsg(_clientToInvite, invitemsg, 0);
	return _errorMessage.empty() ? true : false;
}

Invite::~Invite() {}