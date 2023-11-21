#include "Nick.hpp"
#include "Server.hpp"

bool Nick::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server){
	std::string nickName = tokens[1];
	_errorMessage = "";
	if (tokens.size() < 2)
		_errorMessage = "431 " + client->getHost() + " :No nickname given\r\n";
	else if (isdigit(nickName[0]) || nickName[0] == '#' || nickName[0] == ':' || nickName.find(" ") != std::string::npos)
		_errorMessage = "432 " + client->getHost() + nickName + " :Erroneus nickname\r\n";
	else if (server.nickNameExist(nickName))
		_errorMessage = "433 " + client->getHost() + nickName + " :Nickname is already in use\r\n";
	return _errorMessage.empty() ? true : false;
}

bool Nick::execute(Client *client, std::vector<std::string> tokens, Server &server) {
	std::string newNickName = tokens[1];
	std::string oldNickName = client->getNickname();

	if (!isValidCommand(tokens, client, server))
	{
		nonBlockingSend(client, _errorMessage, 0);
		std::cout << "Error msg sent to client:" << RED << _errorMessage << RESET << std::endl;
	}
	else {
		std::cout << GREEN << "Executing NICK command" << RESET << std::endl;
		client->setNickname(newNickName);
		std::string messageToClient = ":" + oldNickName + " NICK :" + newNickName + "\r\n";
		std::cout << YELLOW << "message sent to client:" << messageToClient << RESET << std::endl;
		nonBlockingSend(client, messageToClient, 0);
	}
	return _errorMessage.empty() ? true : false;
}