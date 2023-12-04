#include "Mode.hpp"
#include "Server.hpp"
#include "Message.hpp"
#include "Channel.hpp"
#include "Client.hpp"

void Mode::setVariableToZero()
{
	_errorMessage = "";
	// _channelName = "";
}

// bool Mode::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server)
// {
// 	(void)server;
// 	if (tokens.size() < 3)
// 		_errorMessage = "461 " + client->getNickname() + " MODE :Not enough parameters\r\n";

// 	return _errorMessage.empty() ? true : false;
// }

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
		// std::cout << GREEN << "Executing MODE i command" << RESET << std::endl;
		if (tokens[2][0] == '-'){
			channel->setJoinOnInvitation(0);
			std::cout << GREEN << "Executing MODE -i command" << RESET << std::endl;
			std::string message = channelName + " :Set open to public\r\n";
			Msg::sendMsg(client, message, 0);
			std::cout << YELLOW << "msg sent to client:"  << _errorMessage << RESET << std::endl;
		}
		else if (tokens[2][0] == '+'){
			channel->setJoinOnInvitation(1);
			std::cout << GREEN << "Executing MODE +i command" << RESET << std::endl;
			std::string message = channelName + " :Set to join on invite only \r\n";
			Msg::sendMsg(client, message, 0);
			std::cout << YELLOW << "msg sent to client:"  << _errorMessage << RESET << std::endl;
		}
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

	if (tokens.size() != 3)
		_errorMessage = "461 " + client->getNickname() + " MODE :Bad number of parameters\r\n";
	else if (tokens[2][0] != '-' && tokens[2][0] != '+')
		_errorMessage = "501 " + client->getNickname() + " MODE " + tokens[2] + " :Unknown MODE flag\r\n";
	else if (!server.isModerator(client, channelName))
		_errorMessage = "482 " + client->getNickname() + " " + channelName + " :You're not channel operator\r\n";
	else if (!server.channelExist(channelName))
		_errorMessage = "403 " + client->getNickname() + channelName + " :No such channel\r\n";
	else if (tokens[2][0] == '+' && !channel->isTopicPublic() )
		_errorMessage = "666 " + client->getNickname() + channelName + " :Is already settable only by operator\r\n";
	else if (tokens[2][0] == '-' && channel->isTopicPublic())
		_errorMessage = "666 " + client->getHost() + channelName + " :Is already settable not only by operator\r\n";
	if (_errorMessage.empty()) {
		if (tokens[2][0] == '-'){
			channel->setTopicPublic(1);
			std::cout << GREEN << "Executing MODE -t command" << RESET << std::endl;
			std::string message = channelName + " :Topic can be change by everyone \r\n";
			Msg::sendMsg(client, message, 0);
			std::cout << YELLOW << "msg sent to client:"  << _errorMessage << RESET << std::endl;
		}
		else if (tokens[2][0] == '+'){
			channel->setTopicPublic(0);
			std::cout << GREEN << "Executing MODE +t command" << RESET << std::endl;
			std::string message = channelName + " :Topic can be change only by operator\r\n";
			Msg::sendMsg(client, message, 0);
			std::cout << YELLOW << "msg sent to client:"  << _errorMessage << RESET << std::endl;
		}
	}
	else {
		Msg::sendMsg(client, _errorMessage, 0);
		std::cout << RED << "Error msg sent to client:"  << _errorMessage << RESET << std::endl;
	}
}

void Mode::setKeyForChannel(std::vector<std::string> &tokens, Client *client, Server &server){
	std::string channelName = tokens[1];
	Channel *channel = server.getChannel(channelName);

	if (tokens.size() != 3 && tokens[2][0] == '-')
		_errorMessage = "461 " + client->getNickname() + " MODE :Bad number of parameters\r\n";
	else if (tokens.size() != 4 && tokens[2][0] == '+')
		_errorMessage = "461 " + client->getNickname() + " MODE :Bad number of parameters\r\n";
	else if (tokens[2][0] != '-' && tokens[2][0] != '+')
		_errorMessage = "501 " + client->getNickname() + " MODE " + tokens[2] + " :Unknown MODE flag\r\n";
	else if (!server.isModerator(client, channelName))
		_errorMessage = "482 " + client->getNickname() + channelName + " :You're not channel operator\r\n";
	else if (!server.channelExist(channelName))
		_errorMessage = "403 " + client->getNickname() + channelName + " :No such channel\r\n";
	if (tokens[2][0] == '-') {
		if (_errorMessage.empty()) {
			std::cout << GREEN << "Executing MODE -k command" << RESET << std::endl;
			channel->setKey("");
			std::string message = channelName + " :Passkey removed\r\n";
			Msg::sendMsg(client, message, 0);
			std::cout << YELLOW << "msg sent to client:"  << _errorMessage << RESET << std::endl;
		}
		else {
			Msg::sendMsg(client, _errorMessage, 0);
			std::cout << RED << "Error msg sent to client:"  << _errorMessage << RESET << std::endl;
		}
	}
	else if (tokens[2][0] == '+') {
		std::string passkey = tokens[3];

		if (passkey.size() > 50)
			_errorMessage = "666 " + client->getNickname() + " " + passkey + " :Passkey too long\r\n";
		if (_errorMessage.empty()) {
			std::cout << GREEN << "Executing MODE +k command" << RESET << std::endl;
			channel->setKey(passkey);
			std::string message = channelName + " :Passkey changed to " + passkey + "\r\n";
			Msg::sendMsg(client, message, 0);
			std::cout << YELLOW << "msg sent to client:"  << _errorMessage << RESET << std::endl;
		}
		else {
			Msg::sendMsg(client, _errorMessage, 0);
			std::cout << RED << "Error msg sent to client:"  << _errorMessage << RESET << std::endl;
		}
	}
}

void Mode::setOperatorForChannel(std::vector<std::string> &tokens, Client *client, Server &server) {
	std::string channelName = tokens[1];
	Channel *channel = server.getChannel(channelName);
	if (tokens.size() != 4){
		_errorMessage = "461 " + client->getNickname() + " MODE :Bad number of parameters\r\n";
		Msg::sendMsg(client, _errorMessage, 0);
		std::cout << RED << "Error msg sent to client:"  << _errorMessage << RESET << std::endl;
		return;
	}

	std::string nickNameToSetOp = tokens[3];
	Client *clientToSetOp = server.getClientByNickname(nickNameToSetOp);

	if (tokens[2][0] != '-' && tokens[2][0] != '+')
		_errorMessage = "501 " + client->getNickname() + " MODE " + tokens[2] + " :Unknown MODE flag\r\n";
	else if (!server.isModerator(client, channelName))
		_errorMessage = "482 " + client->getNickname() + " " + channelName + " :You're not channel operator\r\n";
	else if (!server.channelExist(channelName))
		_errorMessage = "403 " + client->getNickname() + " " + channelName + " :No such channel\r\n";
	else if (!server.isModerator(clientToSetOp, channelName) && tokens[2][0] == '-')
		_errorMessage = "482 " + client->getNickname() + " " + channelName + " " + nickNameToSetOp + " :Is alreadynot a operator\r\n";
	else if (server.isModerator(clientToSetOp, channelName) && tokens[2][0] == '+')
		_errorMessage = "482 " + client->getNickname() + " " + channelName + " " + nickNameToSetOp + " :Is already operator\r\n";
	if (_errorMessage.empty()) {
		if (tokens[2][0] == '-') {
			std::cout << GREEN << "Executing MODE -o command" << RESET << std::endl;
			channel->deleteModerator(client, clientToSetOp);
			std::string message = channelName + " " + nickNameToSetOp + " :Is not a moderator anymore\r\n";
			Msg::sendMsg(client, message, 0);
			std::cout << YELLOW << "msg sent to client:"  << _errorMessage << RESET << std::endl;
		}
		else if (tokens[2][0] == '+'){
			std::cout << GREEN << "Executing MODE +o command" << RESET << std::endl;
			channel->setTopicPublic(0);
			std::string message = channelName + " " + nickNameToSetOp + " :Is now a moderator\r\n";
			Msg::sendMsg(client, message, 0);
			std::cout << YELLOW << "msg sent to client:"  << _errorMessage << RESET << std::endl;
		}
	}
	else {
		Msg::sendMsg(client, _errorMessage, 0);
		std::cout << RED << "Error msg sent to client:"  << _errorMessage << RESET << std::endl;
	}

}

void Mode::setMaxClientForChannel(std::vector<std::string> &tokens, Client *client, Server &server) {
	std::string channelName = tokens[1];
	Channel *channel = server.getChannel(channelName);

	if (tokens.size() != 3 && tokens[2][0] == '-')
		_errorMessage = "461 " + client->getNickname() + " MODE :Bad number of parameters\r\n";
	else if (tokens.size() != 4 && tokens[2][0] == '+')
		_errorMessage = "461 " + client->getNickname() + " MODE :Bad number of parameters\r\n";
	else if (tokens[2][0] != '-' && tokens[2][0] != '+')
		_errorMessage = "501 " + client->getNickname() + " MODE " + tokens[2] + " :Unknown MODE flag\r\n";
	else if (!server.isModerator(client, channelName))
		_errorMessage = "482 " + client->getNickname() + channelName + " :You're not channel operator\r\n";
	else if (!server.channelExist(channelName))
		_errorMessage = "403 " + client->getNickname() + channelName + " :No such channel\r\n";
	if (tokens[2][0] == '-') {
		if (_errorMessage.empty()) {
			std::cout << GREEN << "Executing MODE -l command" << RESET << std::endl;
			channel->setKey("");
			std::string message = channelName + " :Passkey removed\r\n";
			Msg::sendMsg(client, message, 0);
			std::cout << YELLOW << "msg sent to client:"  << _errorMessage << RESET << std::endl;
		}
		else {
			Msg::sendMsg(client, _errorMessage, 0);
			std::cout << RED << "Error msg sent to client:"  << _errorMessage << RESET << std::endl;
		}
	}
	else if (tokens[2][0] == '+') {
		std::string passkey = tokens[3];

		if (passkey.size() > 50)
			_errorMessage = "666 " + client->getNickname() + " " + passkey + " :Passkey too long\r\n";
		if (_errorMessage.empty()) {
			std::cout << GREEN << "Executing MODE +k command" << RESET << std::endl;
			channel->setKey(passkey);
			std::string message = channelName + " :Passkey changed to " + passkey + "\r\n";
			Msg::sendMsg(client, message, 0);
			std::cout << YELLOW << "msg sent to client:"  << _errorMessage << RESET << std::endl;
		}
		else {
			Msg::sendMsg(client, _errorMessage, 0);
			std::cout << RED << "Error msg sent to client:"  << _errorMessage << RESET << std::endl;
		}
	}
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
		if (tokens[2].size() == 2 && tokens[2][1] == 'k')
			setKeyForChannel(tokens, client, server);
		if (tokens[2].size() == 2 && tokens[2][1] == 'o')
			setOperatorForChannel(tokens, client, server);
		if (tokens[2].size() == 2 && tokens[2][1] == 'l')
			setMaxClientForChannel(tokens, client, server);
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