#pragma once
#include <map>
#include "Map.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
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
#include "Map.hpp"
#include "Logger.hpp"
#include "Client.hpp"

using std::string;
class Channel;

class Server
{
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
    // banned clients by ip address
    Map<string, Client *> _bannedClients = Map<string, Client *>();
    Map<string, Channel *> _channels = Map<string, Channel *>();

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
    void closeFds(void);
    bool cleanAll();
    int addClient(int socketClient, fd_set &use);
    int fdSetClientMsgLoop(char *buffer);
    string readClientMsg(Client *client);
    int getAddress(sockaddr_in &sock_addr, socklen_t &size, string &address, string &port);
    int getServerIp(string &ip);
    Client *createOrGetClient(string clientAddress);
    bool isAllowedToConnect(string clientAddress);
    bool isAllowedToMakeRequest(Client *client);
    Channel *isInChannel(Client *client, string &channelName) const;
    Channel *addToChannel(Client *client, string &channelName);
    Channel *addToChannel(Client *client, string &channelName, string &key);
    void addChannel(Channel *newChannel);
    string hashPassword(const std::string &password);

    string getChannelId(const string &channelName, const string &channelKey);
    const std::vector<Channel *> getChannels() const;
    Channel *join(Client *client, std::string &channel);
    Channel *join(Client *client, std::string &channel, std::string &key);
    vector<Channel *> getClientChannels(Client *client);
    vector<Channel *> getClientChannels(std::string &username);
    vector<Client *> getClientsInAChannel(Channel *channel);
    Client *getClient(std::string &username);
    bool isAuthorized(Client *client);
    bool checkAndSetAuthorization(Client *client, const string &rawClientPassword);
    bool isModerator(Client *client, const string &channelName);
    bool isInChannel(Client *client, std::string &channel);
    bool isInChannel(Client *client, std::string &channel, std::string &key);
    bool isInKickChannel(Client *client, std::string &channelName);
    bool isInKickChannel(Client *client, std::string &channelName, std::string &key);
    bool channelExist(std::string &channel);
    bool channelExist(std::string &channel, std::string &key);
    bool hasTopic(std::string &channelName);
    bool hasTopic(string &channelName, string &key);
    bool removeClient(string &username);
    bool removeClient(Client *client);
    bool banClient(Client *client);
    bool nickNameExist(string &ncikName);
    bool userNameExist(string &userName);
    Channel *getChannel(const string &channelName);
    Channel *getChannel(const std::string &channelName, const std::string &key);
    Channel *removeClientFromChannel(Client *client, const std::string &channelName);
    Channel *removeClientFromChannel(Client *client, const std::string &channelName, const std::string &key);
    Channel *kickClientFromChannel(Client *client, std::string &channelName);
    Channel *kickClientFromChannel(Client *client, std::string &channelName, std::string &key);
    Channel *addClientToChannel(Client *client, std::string &channelName);
    Channel *addClientToChannel(Client *client, std::string &channelName, std::string &key);
    Channel *addTopicToChannel(std::string &topic, std::string &channelName);
    Channel *addTopicToChannel(std::string &topic, std::string &channelName, std::string &key);
    Channel *removeTopicFromChannel(std::string &channelName);
    Channel *removeTopicFromChannel(std::string &channelName, std::string &key);
    vector<Client *> removeChannel(std::string &channelName);
    vector<Client *> removeChannel(std::string &channelName, string &key);
};