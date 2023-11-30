#pragma once
#include "Command.hpp"
#include "Client.hpp"

class Invite : public Command {
private:
	std::string _errorMessage;
	std::string _channelName;
	std::string _clientNickToInvite;
	Client *_clientToInvite;

	bool isValidCommand(std::vector<std::string> &tokens, Client *client, Server &server);
	std::string createMessageToClient(Client *client, std::vector<std::string> tokens);
	void setVariableToZero();

public:
	bool execute(Client *client, std::vector<std::string> tokens, Server &server);
	~Invite();
};