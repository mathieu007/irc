#include "Kick.hpp"

void Kick::printTokens(const std::vector<std::string> &result)
{
	std::cout << "Result: ";
	for (std::vector<std::string>::const_iterator it = result.begin(); it != result.end(); ++it)
	{
		std::cout << "[" << *it << "] ";
	}
	std::cout << std::endl;
}

bool Kick::isValidCommand(const std::vector<std::string> &tokens)
{

	if (tokens.size() < 3 || tokens.size() > 4){
		_errorMessage = "461 KICK :Not enought or too much parameters\r\n";
		return false;
	}
	return true;
}

std::string Kick::createKickMessage(Client *client, const std::vector<std::string> &tokens){
	return ":" + client->getNickname() + " KICK " + tokens[1] + " " + client->getNickname() + "\r\n";
}

bool Kick::execute(Client *client, std::vector<std::string> tokens, Server &server){
	(void)server;
	// printTokens(tokens);

	if (!isValidCommand(tokens)) {
		std::cout << RED << "Error sent to client: " << _errorMessage << RESET << std::endl;
		nonBlockingSend(client, _errorMessage, 0);
		return false;
	}
	std::string channelName = tokens[1].substr(1);
	IChannel *channelPtr = server.doesChannelExist(channelName);
	if (!channelPtr){
		_errorMessage = "IRC " + channelName + " :No such channel\r\n";
		std::cout << RED << "Error sent to client: " << _errorMessage << RESET << std::endl;
		nonBlockingSend(client, _errorMessage, 0);
		return false;
	}
	// if (!client->isInChanel(channelName)){
	// 	_errorMessage = "IRC " + client->getNickname() + " " + channelName + " :They aren't on that channel\r\n";
	// 	std::cout << RED << "Error sent to client: " << _errorMessage << RESET << std::endl;
	// 	nonBlockingSend(client, _errorMessage, 0);
	// 	return false;
	// }
	std::cout << GREEN << "Executing KICK command" << RESET << std::endl;
	std::string messageToClient = createKickMessage(client, tokens);
	std::cout << YELLOW << "Message sent to client: " << messageToClient << RESET << std::endl;
	nonBlockingSend(client, messageToClient, 0);

	return true;
}