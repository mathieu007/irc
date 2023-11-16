#pragma once
#include "Client.hpp"
#include "Command.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>


class Nick : public Command {
private:
	std::string _NickMessageToClient;

public:
	// Nick();
	// Nick(std::string newNick, Client &client);
	// ~Nick();

	void execute(Client *client, std::vector<std::string> tokens);
};