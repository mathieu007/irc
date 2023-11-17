#pragma once
#include "Command.hpp"
#include "Message.hpp"

class User : public Command {
private:

public:
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
};