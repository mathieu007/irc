#pragma once
#include <string>
#include <sstream>
#include <iostream>

using std::string;

class Client 
{
    private:
        string _nickname;
        string _username;
        string _pass;
        string _host;
        int _socket;

    public:
        Client();
        void setHost(string host);
        void setSocket(int socket);
        string getHost() const;
        int getSocket() const;
};