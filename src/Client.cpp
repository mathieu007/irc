#include "Client.hpp"
#include "Server.hpp"

void Client::addUser(std::string msg){
	//setnick
	size_t foundPos = msg.find("NICK");
	if (foundPos != std::string::npos){
		this->_nickName = msg.substr(foundPos + 5);
		std::cout << "setnick=" << _nickName << std::endl;
	}
	//setuser
	if (msg.find("USER") != std::string::npos){
		size_t firstSpacePos = msg.find(' ');
		size_t secondSpacePos = msg.find(' ', firstSpacePos + 1);
		std::string result = msg.substr(firstSpacePos + 1, secondSpacePos - firstSpacePos - 1);
		this->_userName = result;
		std::cout << "setUser=" << result << std::endl;
	}
	//join
	if (msg.find("JOIN") != std::string::npos)
	{
		size_t firstSpacePos = msg.find(' ');
		size_t secondSpacePos = msg.find(' ', firstSpacePos + 1);
		std::string channel = msg.substr(firstSpacePos + 1, secondSpacePos - firstSpacePos - 1);
		this->_userName = channel;
		std::cout << "channel=" << channel << std::endl;
		std::string channelMessage = ":" + _nickName + " JOIN " + "#" + channel;
		std::cout << channelMessage << std::endl;
		send(_socket, channelMessage.c_str(), channelMessage.size(), 0);
	}
}
Client::Client(){}


///////////////// SETTER /////////////////

void Client::setHost(string host){
    _host = host;
}

void Client::setSocket(int socket){
    _socket = socket;
}

void Client::setNickname(std::string nickName){
	_nickName = nickName;
}

	////////////////// GETTER ///////////////

	string Client::getHost() const
{
	return _host;
}

int Client::getSocket() const{
    return _socket;
}

std::string Client::getNickname() const {
	return _nickName;
}

std::string Client::getUsername() const {
	return _userName;
}