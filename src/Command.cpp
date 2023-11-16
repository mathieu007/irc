#include <Command.hpp>

Command::Command(){}

// Command::Command(std::string message){
// 	std::istringstream iss(message);

// 	std::cout << "msg receved:" << message << std::endl;
// 	std::string token;
// 	while (std::getline(iss, token, ' ')) {
// 		_tokens.push_back(token);
// 		// std::cout << "Token: " << token << std::endl;
// 	}
// 	for (std::vector<std::string>::iterator it = _tokens.begin(); it != _tokens.end(); ++it) {
// 		std::cout << "token:" << *it << std::endl;
// 	}
// }

Command::~Command(){}