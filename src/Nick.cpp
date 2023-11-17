#include "Nick.hpp"

void Nick::execute(Client *client, std::vector<std::string> tokens) {
	std::cout << GREEN << "Executing NICK command" << RESET << std::endl;

	std::vector<std::string>::iterator it = tokens.begin();
	std::string newNickName = *(it + 1);
	std::string oldNickName = client->getNickname();
	// std::cout << "old nick: " << oldNickName << std::endl;
	client->setNickname(newNickName);
	// std::cout << "settedname:" << client->getNickname() << std::endl;
	std::string messageToClient = ":" + oldNickName + " NICK :" + newNickName + "\r\n";
	std::cout << YELLOW << "message sent to client:" << messageToClient << RESET << std::endl;
	nonBlockingSend(client, messageToClient, 0);
}