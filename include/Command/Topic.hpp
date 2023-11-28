#pragma once
#include "Command.hpp"
#include "Channel.hpp"

class Topic : public Command {
private:
	std::string _errorMessage;
	std::string _channelTopic;
	std::string _channelName;
	Channel *_channel;

	bool isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server);
	void setVariableToZero();

public:
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
	~Topic();
};