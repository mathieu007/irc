#pragma once
#include "Command.hpp"

class User : public Command
{
private:
public:
	~User();
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
};