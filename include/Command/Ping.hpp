#pragma once
#include "Command.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <sys/socket.h>

class Ping : public Command
{

public:
	~Ping();
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
};