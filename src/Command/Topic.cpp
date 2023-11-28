#include "Topic.hpp"
#include "Server.hpp"
#include "Message.hpp"

void Topic::setVariableToZero(){
	_errorMessage = "";
	_channelTopic = "";
	_channelName = "";
	_channel = nullptr;
}

bool Topic::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server) {
	_errorMessage = "";
	
	if (tokens.size() < 2)
		_errorMessage = "461 " + client->getHost() + "TOPIC :Not enought parameters\r\n";
	if (!server.channelExist(_channelName))
		_errorMessage = "403 " + client->getHost() + _channelName + " :No such channel\r\n";
	if (!server.isInChannel(client, _channelName))
		_errorMessage = "442 " + client->getHost() + _channelName + " :You're not on that channel\r\n";
	if (!server.isModerator(client, _channelName))
		_errorMessage = "482 " + client->getHost() + _channelName + " :You're not channel operator\r\n";

	return _errorMessage.empty() ? true : false;
}

bool Topic::execute(Client *client, std::vector<std::string> tokens, Server &server) {
	setVariableToZero();
	if (tokens.size() > 1)
		_channelName = tokens[1];
	if (tokens.size() > 2)
		_channelTopic = tokens[2].substr(1);
	
	if (!isValidCommand(tokens, client, server)){
		Msg::sendMsg(client, _errorMessage, 0);
		std::cout << "Error msg sent to client:" << RED << _errorMessage << RESET << std::endl;
	}
	else {
		std::cout << GREEN << "Executing TOPIC command" << RESET << std::endl;
		_channel = server.getChannel(_channelName);
		_channel->setTopic(_channelTopic);
	}
	return _errorMessage.empty() ? true : false;
}


Topic::~Topic() {}