#pragma once
#include "Command.hpp"

class Part : public Command {
private:
	std::string _errorMessage;
	std::string _channelName;
	std::string _reasonToLeave;

	std::string createReasonMessage(std::vector<std::string> tokens);
	bool isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server);
	std::string createPartMessage(Client *client, const std::vector<std::string> &tokens);
	void setVariableToZero();

public:
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
	~Part();
};