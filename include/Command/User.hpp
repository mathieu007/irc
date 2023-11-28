#pragma once
#include "Command.hpp"

class User : public Command {
private:
	std::string _errorMessage;
	std::string _newUserName;
	std::string _newRealName;
	
public:
	~User();
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
	bool isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server);
};