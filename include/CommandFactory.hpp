#pragma once
#include <iostream>
#include <map>
#include "Command.hpp"
#include "Nick.hpp"
#include "Ping.hpp"

// class Nick;

class CommandFactory
{
private:
	std::map<std::string, Command *> _commandMap;

public:
	CommandFactory();
	~CommandFactory();

	Command *createCommand(const std::string &commandType);
	bool tokenMessage(std::string message, Client *client);
	bool isValid(const std::string &commandType);
};