#include "Ping.hpp"

void Ping::execute(Client *client, std::vector<std::string> tokens){
	if (tokens.size() > 1) {
		std::string messageToClient = "PONG " + tokens[1] + "\r\n";
		std::cout << YELLOW << "message sent to client:" << messageToClient << RESET << std::endl;
		nonBlockingSend(client, messageToClient, 0);
	}
	else
		std::cout << RED << "YOUR CHAT IS HUNTED!!!!" << RESET << std::endl;
}