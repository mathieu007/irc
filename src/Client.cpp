#include "Client.hpp"
#include "Server.hpp"

Client::Client()
{
    _msg = std::string();
	_nickName = "user";
	_isRegistered = 0;
}

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

void Client::setUsername(std::string userName){
	_userName = userName;
}

void Client::setRealname(std::string realName){
	_realName = realName;
}

void Client::setIsRegistered(){
	_isRegistered = true;
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

bool Client::getIsRegistered() const {
	return _isRegistered;
}

string &Client::getMsg()
{
    return _msg;
}

void Client::setMsg(string msg)
{
    _msg = msg;
}