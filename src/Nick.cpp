#include "Nick.hpp"


void Nick::execute(Client *client, std::vector<std::string> tokens) {
	std::cout << "Executing NICK command" << std::endl;

	std::vector<std::string>::iterator it = tokens.begin();
	std::cout << "second token:" << *(it + 1) << std::endl;
	client->setNickname(*(it + 1));
	std::cout << "settedname:" << client->getNickname() << std::endl;
	std::string messtest = ":bozo NICK :yoyo\r\n";
	std::cout << "msstest:" << messtest << std::endl;
	nonBlockingSend(client, messtest, 0);
}