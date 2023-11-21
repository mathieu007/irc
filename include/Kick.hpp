#pragma once
#include "Client.hpp"
#include "Command.hpp"
#include "Message.hpp"
#include "Channel.hpp"


class Kick : public Command {
private:
	std::string _errorMessage;
	std::string _kickReasson;

public:
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
	bool isValidCommand(const std::vector<std::string> &tokens);
	void printTokens(const std::vector<std::string> &result);
	std::string createKickMessage(Client *client, const std::vector<std::string> &tokens);
};