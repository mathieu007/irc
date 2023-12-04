#include "Part.hpp"
#include "Server.hpp"
#include "Message.hpp"

void Part::setVariableToZero()
{
	_errorMessage = "";
}

bool Part::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server)
{
	(void)tokens;
	(void)client;
	(void)server;
	// if (tokens.size() != 2)
	// 	_errorMessage = "461 " + client->getHost() + " KICK :Not enought or too much parameters\r\n";
	return _errorMessage.empty() ? true : false;
}

std::string Part::createPartMessage(Client *client, const std::vector<std::string> &tokens)
{
	(void)client;
	size_t size = tokens.size();
	size_t i = 1;
	string message = "";
	while (i < size)
	{
		message += tokens[i];
		message += " ";
		i++;
	}
	return "PART " + message + "\r\n";
}

bool Part::execute(Client *client, std::vector<std::string> tokens, Server &server)
{
	setVariableToZero();
	if (!isValidCommand(tokens, client, server))
	{
		Msg::sendMsg(client, _errorMessage, 0);
		std::cout << RED << "Error sent to client:" << _errorMessage << RESET << std::endl;
	}
	std::cout << GREEN << "Executing PART command" << RESET << std::endl;
	std::string messageToClient = createPartMessage(client, tokens);
	std::cout << YELLOW << "Message sent to client: " << messageToClient << RESET << std::endl;
	Msg::sendMsg(client, messageToClient, 0);
	server.removeClientFromChannel(client, tokens[1]);
	return _errorMessage.empty() ? true : false;
}

Part::~Part() {}

// leaving reason parsing
// error