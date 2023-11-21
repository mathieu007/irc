#include "Nick.hpp"
#include "IServer.hpp"

bool Nick::isValidCommand(const std::vector<std::string> &tokens, Client *client){

	if (tokens.size() < 2)
		_errorMessage = "431 " + client->getHost() + ":No nickname given\r\n";
	else if (nickNameExist(tokens[1]))
		_errorMessage = "476 " + tokens[1] + " :Bad Channel Mask\r\n";
	return _errorMessage.empty() ? true : false;
}

bool Nick::execute(Client *client, std::vector<std::string> tokens, Server &server) {
	(void)server;

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
	return 1;
}