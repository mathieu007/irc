#include "Ping.hpp"

bool Ping::execute(Client *client, std::vector<std::string> tokens, IServer &server)
{
	(void)server;

	if (tokens.size() > 1) {
		std::string messageToClient = "PONG " + tokens[1] + "\r\n";
		std::cout << YELLOW << "message sent to client:" << messageToClient << RESET << std::endl;
		nonBlockingSend(client, messageToClient, 0);
		return 1;
	}
	else {
		std::cout << RED << "YOUR CHAT IS HUNTED!!!!" << RESET << std::endl;
		return 0;
	}
}