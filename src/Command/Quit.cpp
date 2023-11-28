#include "Quit.hpp"
#include "Server.hpp"
#include "Message.hpp"

Quit::~Quit() {}

std::string Quit::createQuitMessage(Client *client, const std::vector<std::string> &tokens)
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
	return "QUIT :" + message + "\r\n";
}

bool Quit::execute(Client *client, std::vector<std::string> tokens, Server &server)
{
	(void)server;
	std::cout << GREEN << "Executing QUIT command" << RESET << std::endl;
	std::string messageToClient = createQuitMessage(client, tokens);
	std::cout << YELLOW << "Message sent to client: " << messageToClient << RESET << std::endl;
	Msg::sendMsg(client, messageToClient, 0);
	server.disconnect(client);
	return true;
}