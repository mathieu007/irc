#pragma once
#include "Command.hpp"

class Join : public Command {
private:
	std::string _errorMessage;
	std::string _channelName;
	std::string _channelKey;
	bool _hasKey;

	bool isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server);

public:
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
	~Join();
};