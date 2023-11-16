#include "CommandFactory.hpp"

CommandFactory::CommandFactory (){
	_commandMap["NICK"] = new Nick();
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
	else {
		std::cout << "Unknown command type: " << commandType << std::endl;
		return nullptr;
	}
}

CommandFactory::~CommandFactory() {
	for (std::map<std::string, Command *>::iterator it = _commandMap.begin(); it != _commandMap.end(); ++it) {
		delete it->second;
	}
}