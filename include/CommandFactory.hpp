#pragma once
#include <iostream>
#include <map>
#include "Command.hpp"
#include "Server.hpp"
#include "Nick.hpp"
#include "Ping.hpp"
#include "User.hpp"
#include "Join.hpp"
#include "Kick.hpp"
#include "Part.hpp"

class CommandFactory {
private:
	std::map<std::string, Command *> _commandMap;

public:
	CommandFactory();
	~CommandFactory();

	Command *createCommand(const std::string &commandType);
	bool tokenMessage(std::string message, Client *client, Server &server);
	bool isValid(const std::string &commandType);
};