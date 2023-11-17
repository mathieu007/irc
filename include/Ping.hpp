#pragma once
#include "Client.hpp"
#include "Command.hpp"
#include "Message.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <sys/socket.h>

class Ping : public Command {

public:
	void execute(Client *client, std::vector<std::string> tokens);
};