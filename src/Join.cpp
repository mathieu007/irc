#include "Join.hpp"

bool Join::execute(Client *client, std::vector<std::string> tokens, Server &server){
	(void)server;
	(void)tokens;
	(void)client;

	//split on "," for multiple channel
	std::vector<std::string> result;
	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
		std::string token = *it;

		std::istringstream iss(token);
		std::string element;

		while (std::getline(iss, element, ','))
		{
			result.push_back(element);
		}
	}

	// Print the result
	std::cout << "Result: ";
	for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it) {
		std::cout << "[" << *it << "] ";
	}
	std::cout << std::endl;

	if (tokens[1][0] != '#'){
		std::string errorMessage = "476 " + tokens[1] + " :Bad Channel Mask\r\n";
		std::cout << RED << "Error sent to client:" << errorMessage << RESET << std::endl;
		nonBlockingSend(client, errorMessage, 0);
		return 0;
	}

	std::string messageToClient = ":" + client->getNickname() + " JOIN :" + tokens[1] + "\r\n";
	std::cout << YELLOW << "message sent to client:" << messageToClient << RESET << std::endl;
	nonBlockingSend(client, messageToClient, 0);

	///add channel to client

	return 1;
}