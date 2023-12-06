#include "CommandFactory.hpp"
#include "Nick.hpp"
#include "Ping.hpp"
#include "User.hpp"
#include "Join.hpp"
#include "Kick.hpp"
#include "Quit.hpp"
#include "Part.hpp"
#include "Pass.hpp"
#include "Privmsg.hpp"
#include "Mode.hpp"
#include "Topic.hpp"
#include "Invite.hpp"

CommandFactory::CommandFactory()
{
	_commandMap["NICK"] = new Nick();
	_commandMap["PING"] = new Ping();
	_commandMap["USER"] = new User();
	_commandMap["JOIN"] = new Join();
	_commandMap["KICK"] = new Kick();
	_commandMap["PART"] = new Part();
	_commandMap["PASS"] = new Pass();
	_commandMap["QUIT"] = new Quit();
	_commandMap["INVITE"] = new Invite();
	_commandMap["PRIVMSG"] = new Privmsg();
	_commandMap["TOPIC"] = new Topic();
	_commandMap["MODE"] = new Mode();
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

bool CommandFactory::isValid(const std::string &commandType)
{
	std::map<std::string, Command *>::iterator it = _commandMap.find(commandType);
	if (it != _commandMap.end())
		return 1;
	else
		return 0;
}

bool CommandFactory::tokenMessage(std::string message, Client *client, Server &server)
{
	(void)server;
	/////////print msg
	std::cout << BLUE << "msg receved from client:" << message << RESET << std::endl;

	/////split if there mutipule commands
	// message = "CAP LS 302\r\nNICK user\r\nUSER user 0 * :user\r\n";
	std::vector<std::string> commands;
	std::string currentCommand;
	for (std::string::size_type i = 0; i < message.length(); ++i) {
		if (message[i] == '\r' && i + 1 < message.length() && message[i + 1] == '\n')
		{
			commands.push_back(currentCommand);
			currentCommand.clear();
			++i;
		}
		else
		{
			currentCommand += message[i];
		}
	}

	/////split each command into tokens
	std::vector<std::vector<std::string>> tokensList;
	for (std::size_t i = 0; i < commands.size(); ++i) {
		std::vector<std::string> tokens;
		std::istringstream iss(commands[i]);
		std::string token;

		while (std::getline(iss, token, ' ')) {
			size_t pos = token.find('\r');
			if (pos != std::string::npos) {
				token.erase(pos, 1);
			}
			pos = token.find('\n');
			if (pos != std::string::npos) {
				token.erase(pos, 1);
			}
			else if (!token.empty()) {
                tokens.push_back(token);
            }
		}
		tokensList.push_back(tokens);
	}

	/////print all command and tokens
	for (std::size_t i = 0; i < tokensList.size(); ++i) {
		std::cout << "Tokens for Command " << i + 1 << ": ";
		for (std::size_t j = 0; j < tokensList[i].size(); ++j)
		{
			std::cout << "[" << tokensList[i][j] << "] ";
		}
		std::cout << std::endl;
	}

	/////execute all commands
	for (std::size_t i = 0; i < tokensList.size(); ++i) {
		if (isValid(tokensList[i].front()))
			_commandMap[tokensList[i].front()]->execute(client, tokensList[i], server);
		else
			std::cout << RED << "Bad command:" << tokensList[i].front() << RESET << std::endl;
	}
	return 1;
}

CommandFactory::~CommandFactory()
{
	for (std::map<std::string, Command *>::iterator it = _commandMap.begin(); it != _commandMap.end(); ++it)
	{
		delete it->second;
	}
}