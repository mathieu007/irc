#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "Client.hpp"
#include "IServer.hpp"
// #include "Message.hpp"



#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"


class Server;

class Command {
private:
	std::vector<std::string> _tokens;

public:
	Command();
	// Command(std::string message);
	virtual ~Command();

	virtual bool execute(Client *client, std::vector<std::string> tokens, IServer &server) = 0;
};