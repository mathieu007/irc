#pragma once
#include "Client.hpp"
#include "Command.hpp"
#include "Message.hpp"
#include "Channel.hpp"


class Join : public Command {
private:
	std::string _errorMessage;

	std::vector<std::string> splitTokens(const std::vector<std::string> &tokens);
	void printTokens(const std::vector<std::string> &result);
	// bool isValidChannel(const std::vector<std::string> &tokens);
	// std::string createErrorMessage(const std::vector<std::string> &tokens);
	std::string createJoinMessage(Client *client, const std::vector<std::string> &tokens);
	bool isValidCommand(const std::vector<std::string> &tokens);

public:
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
};