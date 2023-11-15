#include "Client.hpp"

Client::Client()
{
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