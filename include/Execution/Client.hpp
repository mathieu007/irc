#pragma once

#define MAX_CLIENTS FD_SETSIZE
#define MAX_BUFFER_SIZE 8096
#define MAX_REQ_PER_SEC 15
#define MAX_REQ_BEFORE_BAN 60
#define MAX_REQ_SIZE_PER_SEC 8096

#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#include "Map.hpp"
#include <vector>
#include <chrono>
#include <iomanip>

using std::string;
using std::vector;

class Channel;

class Client
{
private:
    string _nickName;
    string _userName;
    string _realName;
    string _pass;
    string _host;
    string _msg;
    // for a 100% non blocking io, while we send data to multiple recipients we need to use the main loop and not use sendMsg otherwise, it's why we need a queue...
    string _msgQueue;
    int _socket;
    bool _isBanned;
    long _lastRequestTime;
    long _nextAllowedConnectionTime;
    int _numRequests;
    int _reqSize;
    string _address;
    string _port;
    // channels by channelnameKey
    Map<string, Channel *> _channels;
    vector<Channel *> _kickedChannels;
    bool _isRegistered;

public:
    Client();
    bool operator==(const Client &cmp) const;
    bool operator!=(const Client &cmp) const;
    ~Client();
    string getNickname() const;
    const string &getUsername() const;
    const string &getPass() const;
    long getCurTime() const;
    int getSocket() const;
    string getHost() const;
    string &getMsg();
    string &getMsgQueue();
    bool isRegistered() const;
    Map<string, Channel *> &getChannels();
    vector<Channel *> &getKickedChannels();
    void setHost(string host);
    void setSocket(int socket);
    void setMsg(string msg);
    void setMsgQueue(string msg);
    void setPass(string pass);
    void setPort(string port);
    void setAddress(string address);
    void setNickname(std::string ncikName);
    void setUsername(std::string userName);
    void setRealname(std::string realName);
    void setIsRegistered();
    void incrementRequest();
    void incrementReqSize(long reqSize);
    bool isAuthenticated() const;
    bool passIsEmpty() const;
    bool canConnect() const;
    bool isBannned() const;
    bool canMakeRequest();
    bool isGoingToGetBanned();

    // WARNING do not use these functions, use the server's functions instead, these functions are only called from the server.
    bool addToChannel(Channel *channel);
    bool addToKickedChannel(Channel *channel);
    bool removeFromChannel(Channel *channel);
    bool removeFromKickChannel(Channel *channel);
    bool isInChannel(Channel *channel);
    bool isInKickChannel(Channel *channel);
};
