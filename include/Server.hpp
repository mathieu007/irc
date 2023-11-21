#pragma once


#include <map>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <csignal>
#include <cerrno>
#include <fcntl.h>
#include <algorithm>
#include "String.hpp"
#include "Logger.hpp"
#include "Message.hpp"
#include "Channel.hpp"



class Server {
private:
    std::string _hostname;
    std::string _hostIp;
    Logger _logger;
    int _max_fd_set;
    std::string _pass;
    int _port;
    struct sockaddr_in _serv_addr;
    struct sockaddr_in _client_adrr;
    socklen_t _serv_size;
    socklen_t _client_size;
    int _socket;

    /// @brief non blocking io, for monitoring both read and write operations.
    fd_set _use;
    /// @brief non blocking io for writing.
    fd_set _writing;
    /// @brief non blocking io for reading.
    fd_set _reading;

    vector<Client *> _clients = vector<Client *>(MAX_CLIENTS);
    // clients banned by ip address without port
    std::map<string, Client *> _bannedClients = std::map<string, Client *>();
    vector<IChannel *> _channels = vector<IChannel *>();
    std::map<string,string> _channelKeys = std::map<string,string>();

    int _setSockAddrStorage();
    string _getHostname() const;
    void _initServerSocket(void);
    void _setNonBlocking(int sockfd);
    int _selectFdSet();


public:
    Server(char *pass, int port, bool fileLog);
    Server(char *pass, int port, char *ip, bool fileLog);
    ~Server();
    Server(const Server &serv);
    Server &operator=(const Server &serv);
    int acceptClient();
    void initServer(void);
    void closeServer(void);
    int addClient(int socketClient, fd_set &use);
    int fdSetClientMsgLoop(char *buffer);
    string readClientMsg(Client *client);
    int getAddress(sockaddr_in &sock_addr, socklen_t &size, string &address, string &port);
    int getServerIp(string &ip);
    Client *createOrGetClient(string clientAddress);
    bool isAllowedToConnect(string clientAddress);
    bool isAllowedToMakeRequest(Client *client);
    bool nickNameInUse() const;
    bool userNameInUse(string &userName);
    IChannel *isInChannel(Client *client, string &channelName) const;
    IChannel *addToChannel(Client *client, string &channelName);
    IChannel *addToChannel(Client *client, string &channelName, string &key);

	IChannel *doesChannelExist(const std::string &channelName) const;
	void addChannel(IChannel *newChannel);
	const std::vector<IChannel *> &getChannels() const;
};
