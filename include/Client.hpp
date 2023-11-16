#pragma once
#include <string>
#include <sstream>
#include <iostream>
#define MAX_BUFFER_SIZE 8096

using std::string;

class Client 
{
    private:
        string _nickname;
        string _username;
        string _pass;
        string _host;
        string _msg;
        int _socket;

    public:
        Client();
        void setHost(string host);
        void setSocket(int socket);
        string getHost() const;
        int getSocket() const;
        string &getMsg();
        void setMsg(string msg);
};