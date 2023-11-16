#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "Client.hpp"
// #include "Nick.hpp"

class Command {
private:
	std::vector<std::string> _tokens;

public:
	Command();
	// Command(std::string message);
	virtual ~Command();

	virtual void execute(Client *client, std::vector<std::string> tokens) = 0;
};