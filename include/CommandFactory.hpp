#pragma once
#include <iostream>
#include <Command.hpp>
#include <map>
#include "Nick.hpp"

// class Nick;

class CommandFactory
{
private:
	std::map<std::string, Command *> _commandMap;

public:
	CommandFactory();
	Command *createCommand(const std::string &commandType);
	~CommandFactory();
};