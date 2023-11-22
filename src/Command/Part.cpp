#include "Part.hpp"
#include "Server.hpp"
#include "Message.hpp"

Part::~Part() {}

std::string Part::createPartMessage(Client *client, const std::vector<std::string> &tokens)
{
	return ":" + client->getNickname() + " PART " + tokens[1] + " " + tokens[2] + "\r\n";
}

bool Part::execute(Client *client, std::vector<std::string> tokens, Server &server)
{
	(void)server;
	std::cout << GREEN << "Executing PART command" << RESET << std::endl;
	std::string messageToClient = createPartMessage(client, tokens);
	std::cout << YELLOW << "Message sent to client: " << messageToClient << RESET << std::endl;
	sendMsg(client, messageToClient, 0);
	return true;
}