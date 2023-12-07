#pragma once
#include "Command.hpp"
#include "Client.hpp"

class Kick : public Command {
private:
	std::string _errorMessage;
	std::string _kickReasson;
	std::string _channelName;
	std::string _clientNickToKick;
	Client *_clientToKick;

	std::string createReasonMessage(std::vector<std::string> tokens);
	bool isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server);
	std::string createMessageToClient(Client *client, std::vector<std::string> tokens);
	void setVariableToZero();

public:
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
	~Kick();
};