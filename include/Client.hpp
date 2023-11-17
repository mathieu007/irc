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
    string _username;
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

public:
    Client();
    void setHost(string host);
    void setSocket(int socket);
    string getHost() const;
    int getSocket() const;
    string &getMsg();

    void setNickname(std::string ncikName);
    string getNickname() const;
    string getUsername() const;
    void setMsg(string msg);
    void setPort(string port);
    void setAddress(string address);

    long getCurTime() const;
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