#pragma once
#include "Command.hpp"
// #include "Client.hpp"

class Nick : public Command {
private:
	std::string _errorMessage;
	std::string _newNickName;
	std::string _oldNickName;
	
	bool isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server);

public:
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
	~Nick();
};