#pragma once
#include "Client.hpp"
#include "Command.hpp"
#include "Message.hpp"
#include "Channel.hpp"


class Join : public Command {
private:
	std::string _errorMessage;

	void printTokens(const std::vector<std::string> &result);
	std::string createJoinMessage(Client *client, const std::vector<std::string> &tokens);
	bool isValidCommand(const std::vector<std::string> &tokens);

public:
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
};