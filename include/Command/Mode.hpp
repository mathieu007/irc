#pragma once
#include "Command.hpp"

class Mode : public Command {
private:
	std::string _errorMessage;
	// std::string _channelName;

	void setKeyForChannel(std::vector<std::string> &tokens, Client *client, Server &server);
	void setTopicByOperatorOnly(std::vector<std::string> &tokens, Client *client, Server &server);
	void setInviteOnly(std::vector<std::string> &tokens, Client *client, Server &server);
	bool isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server);
	void setVariableToZero();
	
public:
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
	~Mode();
};