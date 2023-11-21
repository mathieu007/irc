#pragma once

#define MAX_CLIENTS FD_SETSIZE
#define MAX_BUFFER_SIZE 8096
#define MAX_REQ_PER_SEC 4
#define MAX_REQ_BEFORE_BAN 30
#define MAX_REQ_SIZE_PER_SEC 8096

#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#include "IChannel.hpp"
#include <vector>
#include <chrono>

using std::string;
using std::vector;

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
    bool _banned;
    long _lastRequestTime;
    long _nextAllowedConnectionTime;
    int _numRequests;
    int _reqSize;
    string _address;
    string _port;
    vector<IChannel *> _registeredChannels;
	bool _isRegistered;

public:
    Client();

    string getNickname() const;
    string getUsername() const;
    long getCurTime() const;
    int getSocket() const;
    string getHost() const;
    string &getMsg();
	bool getIsRegistered() const;

    void setHost(string host);
    void setSocket(int socket);
    void setMsg(string msg);
    void setPort(string port);
    void setAddress(string address);
    void setNickname(std::string ncikName);
	void setUsername(std::string userName);
	void setRealname(std::string realName);
	void setIsRegistered();

    void incrementRequest();
    void incrementReqSize(long reqSize);

    bool userNameInUse() const;
    bool nickNameInUse() const;
    bool passwordsMatches(string toCompare) const;
    bool canConnect() const;
    bool isBannned() const;
    bool canMakeRequest();
    bool isGoingToGetBanned();

    IChannel *addToChanel(string &channelName);
    bool removeFromChanel();
    IChannel *isInChanel(string &channelName) const;
    IChannel *findByName(string &channelName) const;
};


// void addUser(std::string msg);
