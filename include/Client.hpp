#pragma once
#include <string>
#include <sstream>
#include <iostream>
#define MAX_BUFFER_SIZE 8096

using std::string;

class Client 
{
    private:
        string _nickName;
        string _userName;
		string _realName;
        string _pass;
        string _host;
        string _msg;
        int _socket;
		bool _isRegistered;

	public:
        Client();
		
        void setHost(string host);
        void setSocket(int socket);
		void setNickname(std::string nickName);
		void setUsername(std::string userName);
		void setRealname(std::string realName);
		void setIsRegistered();

		string getHost() const;
        int getSocket() const;
		std::string getNickname() const;
		std::string getUsername() const;
		bool getIsRegistered() const;

		void addUser(std::string msg);
        string &getMsg();
        void setMsg(string msg);
};