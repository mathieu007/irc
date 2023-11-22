#include "Nick.hpp"
#include "Server.hpp"
#include "Message.hpp"

bool Nick::isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server)
{
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

// 	if (tokens.size() < 2)
// 		_errorMessage = "431 " + client->getHost() + ":No nickname given\r\n";
// 	else if (nickNameExist(tokens[1]))
// 		_errorMessage = "476 " + tokens[1] + " :Bad Channel Mask\r\n";
// 	return _errorMessage.empty() ? true : false;
// }
Nick::~Nick() {}
bool Nick::execute(Client *client, std::vector<std::string> tokens, Server &server)
{
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
	sendMsg(client, messageToClient, 0);
	return 1;
}