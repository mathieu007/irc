#include "Client.hpp"

Client::Client()
{
    _msg = std::string();
}
void Client::setHost(string host)
{
    _host = host;
}
void Client::setSocket(int socket)
{
    _socket = socket;
}
string Client::getHost() const
{
    return _host;
}
int Client::getSocket() const
{
    return _socket;
}
string &Client::getMsg()
{
    return _msg;
}

void Client::setMsg(string msg)
{
    _msg = msg;
}

void Client::setPort(string port)
{
    _port = port;
}

void Client::setAddress(string address)
{
    _address = address;
}