#include "CommandFactory.hpp"

CommandFactory::CommandFactory (){
	_commandMap["NICK"] = new Nick();
	_commandMap["PING"] = new Ping();
	// commandMap["JOIN"] = new Join();
	// commandMap["USER"] = new User();
}

Command *CommandFactory::createCommand(const std::string &commandType)
{
	std::map<std::string, Command *>::iterator it = _commandMap.find(commandType);
	if (it != _commandMap.end())
	{
		return it->second;
	}
	else
	{
		std::cout << "Unknown command type: " << commandType << std::endl;
		return nullptr;
	}
}

bool CommandFactory::isValid(const std::string &commandType){
	std::map<std::string, Command *>::iterator it = _commandMap.find(commandType);
	if (it != _commandMap.end())
		return 1;
	else
		return 0;
}

	bool CommandFactory::tokenMessage(std::string message, Client *client)
{
	/////////parse msg
	(void)client;
	std::vector<std::string> _tokens;
	std::istringstream iss(message);

	/////////print msg
	std::cout << BLUE << "msg receved from client:" << message << RESET << std::endl;
	std::string token;
	while (std::getline(iss, token, ' '))
	{
		size_t pos = token.find('\n');
		if (pos != std::string::npos){
			token.erase(pos, 1);
		}
		pos = token.find('\r');
		if (pos != std::string::npos){
			token.erase(pos, 1);
		}
		_tokens.push_back(token);
		// std::cout << "Token: " << token << std::endl;
	}
	// std::cout << "tokenfront:" << _tokens.front() << std::endl;
	if (isValid(_tokens.front()))
		_commandMap[_tokens.front()]->execute(client, _tokens);
	else
		std::cout << RED  << "Bad command:" << _tokens.front() << RESET << std::endl;
	return 1;
}

CommandFactory::~CommandFactory() {
	for (std::map<std::string, Command *>::iterator it = _commandMap.begin(); it != _commandMap.end(); ++it) {
		delete it->second;
	}
}