#include "Join.hpp"

// bool Join::execute(Client *client, std::vector<std::string> tokens, Server &server){
// 	(void)server;
// 	(void)tokens;
// 	(void)client;

// 	//split on "," for multiple channel
// 	std::vector<std::string> result;
// 	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
// 		std::string token = *it;

// 		std::istringstream iss(token);
// 		std::string element;

// 		while (std::getline(iss, element, ','))
// 		{
// 			result.push_back(element);
// 		}
// 	}

// 	// Print the result
// 	std::cout << "Result: ";
// 	for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it) {
// 		std::cout << "[" << *it << "] ";
// 	}
// 	std::cout << std::endl;

// 	if (tokens[1][0] != '#'){
// 		std::string errorMessage = "476 " + tokens[1] + " :Bad Channel Mask\r\n";
// 		std::cout << RED << "Error sent to client:" << errorMessage << RESET << std::endl;
// 		nonBlockingSend(client, errorMessage, 0);
// 		return 0;
// 	}

// 	std::string messageToClient = ":" + client->getNickname() + " JOIN :" + tokens[1] + "\r\n";
// 	std::cout << YELLOW << "message sent to client:" << messageToClient << RESET << std::endl;
// 	nonBlockingSend(client, messageToClient, 0);

// 	///add channel to client

// 	std::string channelName = tokens[1].substr(1);

// 	IChannel *channelPtr = server.doesChannelExist(channelName);
// 	if (channelPtr){
// 		channelPtr->addClient(client);
// 	}
// 	else {
// 		Channel *newChannel = new Channel(channelName);
// 		newChannel->addClient(client);
// 		server.addChannel(newChannel);
// 	}

// 	return 1;
// }

std::vector<std::string> Join::splitTokens(const std::vector<std::string> &tokens){
	std::vector<std::string> result;
	for (std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		std::istringstream iss(*it);
		std::string element;
		while (std::getline(iss, element, ','))
		{
			result.push_back(element);
		}
	}
	return result;
}

void Join::printTokens(const std::vector<std::string> &result){
	std::cout << "Result: ";
	for (std::vector<std::string>::const_iterator it = result.begin(); it != result.end(); ++it)
	{
		std::cout << "[" << *it << "] ";
	}
	std::cout << std::endl;
}

// bool Join::isValidChannel(const std::vector<std::string> &tokens){
// 	return tokens.size() < 4 && !tokens.empty() && tokens[1][0] == '#';
// }

// std::string Join::createErrorMessage(const std::vector<std::string> &tokens){
// 	return  "476 " + tokens[1] + " :Bad Channel Mask\r\n";
// }

std::string Join::createJoinMessage(Client *client, const std::vector<std::string> &tokens){
	return ":" + client->getNickname() + " JOIN :" + tokens[1] + "\r\n";
}

bool Join::isValidCommand(const std::vector<std::string> &tokens){

	if (tokens.size() < 2 || tokens.size() > 3){
		_errorMessage = "461 JOIN :Not enought or too much parameters\r\n";
		return false;
	}
	if (tokens[1][0] != '#'){
		_errorMessage = "476 " + tokens[1] + " :Bad Channel Mask\r\n";
		return false;
	}
	return true;
}

bool Join::execute(Client *client, std::vector<std::string> tokens, Server &server){

	// printTokens(tokens);

	if (!isValidCommand(tokens)) {
		std::cout << RED << "Error sent to client: " << _errorMessage << RESET << std::endl;
		nonBlockingSend(client, _errorMessage, 0);
		return false;
	}
	std::cout << GREEN << "Executing JOIN command" << RESET << std::endl;
	std::string messageToClient = createJoinMessage(client, tokens);
	std::cout << YELLOW << "Message sent to client: " << messageToClient << RESET << std::endl;
	nonBlockingSend(client, messageToClient, 0);

	std::string channelName = tokens[1].substr(1);

	IChannel *channelPtr = server.doesChannelExist(channelName);
	if (channelPtr){
		channelPtr->addClient(client);
	}
	else {
		Channel *newChannel = new Channel(channelName);
		newChannel->addClient(client);
		server.addChannel(newChannel);
	}

	return true;
}