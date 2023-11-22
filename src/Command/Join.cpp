#include "Join.hpp"
#include "Server.hpp"
#include "Message.hpp"

Join::~Join() {}
void Join::printTokens(const std::vector<std::string> &result)
{
	std::cout << "Result: ";
	for (std::vector<std::string>::const_iterator it = result.begin(); it != result.end(); ++it)
	{
		std::cout << "[" << *it << "] ";
	}
	std::cout << std::endl;
}

std::string Join::createJoinMessage(Client *client, const std::vector<std::string> &tokens)
{
	return ":" + client->getNickname() + " JOIN :" + tokens[1] + "\r\n";
}

bool Join::isValidCommand(const std::vector<std::string> &tokens)
{

	if (tokens.size() < 2 || tokens.size() > 3)
	{
		_errorMessage = "461 JOIN :Not enought or too much parameters\r\n";
		return false;
	}
	if (tokens[1][0] != '#')
	{
		_errorMessage = "476 " + tokens[1] + " :Bad Channel Mask\r\n";
		return false;
	}
	return true;
}

bool Join::execute(Client *client, std::vector<std::string> tokens, Server &server)
{

	// printTokens(tokens);

	if (!isValidCommand(tokens))
	{
		std::cout << RED << "Error sent to client: " << _errorMessage << RESET << std::endl;
		sendMsg(client, _errorMessage, 0);
		return false;
	}
	std::cout << GREEN << "Executing JOIN command" << RESET << std::endl;
	std::string messageToClient = createJoinMessage(client, tokens);
	std::cout << YELLOW << "Message sent to client: " << messageToClient << RESET << std::endl;
	sendMsg(client, messageToClient, 0);
	std::string channelName = tokens[1].substr(1);
	server.join(client, channelName);
	return true;
}