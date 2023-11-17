#pragma once
#include "Client.hpp"
#include "Command.hpp"
#include "Message.hpp"


class Join : public Command {
private:

public:
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
};