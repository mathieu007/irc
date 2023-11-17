#include "User.hpp"

bool User::execute(Client *client, std::vector<std::string> tokens, Server &server) {
	(void)server;
	(void)tokens;

	if (tokens.size() != 5){
		std::string errorMessage = "461 USER :Not enough parameters\r\n";
		nonBlockingSend(client, errorMessage, 0);
		return 0;
	}
	if (client->getIsRegistered()){
		std::string errorMessage = "462 :You may not reregister\r\n";
		nonBlockingSend(client, errorMessage, 0);
		return 0;
	}
	std::cout << GREEN << "Executing USER command" << RESET << std::endl;
	std::string newUserName = tokens[1];
	std::string newRealName = tokens[4].substr(1);
	client->setUsername(newUserName);
	client->setRealname(newRealName);
	client->setIsRegistered();
	return 1;
}