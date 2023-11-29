#pragma once

#define MAX_CLIENT_INACTIVITY 60 * 5
#define MAX_CLIENT_PER_CHANNEL 50

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

using std::cout;
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
    int _serverSocket;
    /// @brief non blocking io, for monitoring both read and write operations.
    fd_set _use;
    /// @brief non blocking io for writing.
    fd_set _writing;
    /// @brief non blocking io for reading.
    fd_set _reading;
    vector<int> _clientSockets = vector<int>();
    vector<Client *> _clients = vector<Client *>(MAX_CLIENTS);
    // banned clients by ip address
    Map<string, Client *> _bannedClients = Map<string, Client *>();
    Map<string, Channel *> _channels = Map<string, Channel *>();

    int _setSockAddrStorage();
    string _getHostname() const;
    void _initServerSocket(void);
    void _setNonBlocking(int sockfd);
    int _selectFdSet();
    void _banClient(Client *client, int clientSocket);
    void _disconnectInnactiveClient(Client *client, int index);
    string _recvClientMsg(Client *client, char *buffer, int clientSocket);
    void _execUnAuthenticatedCmd(string &msg, Client *client);

public:
    Server();
    Server(char *pass, int port, bool fileLog);
    Server(char *pass, int port, char *ip, bool fileLog);
    ~Server();
    Server(const Server &serv);
    Server &operator=(const Server &serv);
    int acceptClient();
    void initServer(void);
    void closeFds(void);
    bool cleanAll();
    int createClient(int socketClient);
    int fdSetClientMsgLoop(char *buffer);
    void CleanServer();

    string readClientMsg(Client *client);
    int getAddress(sockaddr_in &sock_addr, socklen_t &size, string &address, string &port);
    int getServerIp(string &ip);
    vector<int> &getClientSockets();
    vector<Client *> &getClients();
    int checkIncomingClientConnection();
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
    Channel *join(Client *client, std::string &channel, string &key);
    bool disconnect(Client *client);
    vector<Channel *> getClientChannels(Client *client);
    vector<Channel *> getClientChannels(std::string &username);
    vector<Client *> getClientsInAChannel(Channel *channel);
    Client *getClient(std::string &username);
    bool isChannelFull(string &channelName);
    bool isValidPassword(Client *client);
    bool isAuthenticated(Client *client);
    bool setClientPassword(Client *client, const string &rawClientPassword);
    bool isModerator(Client *client, const string &channelName);
    bool isInChannel(Client *client, std::string &channel);
    bool isKickedFromChannel(Client *client, std::string &channelName);
    bool channelExist(std::string &channel);
    bool hasTopic(std::string &channelName);
    bool removeClient(string &username);
    bool removeClient(Client *client);
    bool banClient(Client *client);
    bool nickNameExist(string &ncikName);
    bool userNameExist(string &userName);
    const string &getChannelKey(std::string &channelName);
    bool channelKeyExist(std::string &channelName, std::string &key);
    Channel *getChannel(const string &channelName);
    Channel *removeClientFromChannel(Client *client, const std::string &channelName);
    Channel *kickClientFromChannel(Client *client, std::string &channelName);
    Channel *addClientToChannel(Client *client, std::string &channelName);
    Channel *addTopicToChannel(std::string &topic, std::string &channelName);
    Channel *removeTopicFromChannel(std::string &channelName);
    vector<Client *> removeChannel(std::string channelName);
    Client *getClientByNickname(std::string &nickname);
};