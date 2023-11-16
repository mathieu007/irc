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

class Nick : public Command {
private:
	std::string _NickMessageToClient;

public:
	void execute(Client *client, std::vector<std::string> tokens);
};