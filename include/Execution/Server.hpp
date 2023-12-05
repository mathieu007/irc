#pragma once

#define MAX_CLIENT_INACTIVITY 60 * 5
#define MAX_CLIENT_CONNECTION_RETRY_TIME 5
#define MAX_CLIENT_CONNECTION_RETRY_TIME_STR "5"
#define NEXT_ALLOWED_CONNECTION_TIME_ONCE_BAN (long)(60 * 10)

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
#include "ClientChannelMapping.hpp"
#include "Client.hpp"
#include "Vec.hpp"

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

    /// @brief non blocking io for writing.
    fd_set _writing;
    /// @brief non blocking io for reading.
    fd_set _reading;

    vector<int> _clientSockets = vector<int>();

    // banned clients by ip address + port number, but in real application i would banned based on ip address only, but it's not really usefful while testing...
    Map<string, long> _bannedClients = Map<string, long>();
    Map<string, long> _connectionsLog = Map<string, long>();
    // I choose to centralizing the data, it's a bit slower but much more manageable...
    // for faster operation i would have created a mapping table for clients and channels, this would make read operations faster,
    // also we coud have used indexes to make query faster, but it's fast enough...
    Vec<Client> _clients = Vec<Client>(MAX_CLIENTS);
    Vec<Channel> _channels = Vec<Channel>();    
    Vec<ClientChannelMapping> *_clientschannelsMapping = new Vec<ClientChannelMapping>();

    int _setSockAddrStorage();
    string _getHostname() const;
    void _initServerSocket(void);
    void _setNonBlocking(int sockfd);
    int _selectFdSet();
    void _banClient(Client *client);
    void _disconnectInnactiveClient(Client *client);
    bool _recvClientMsg(Client *client, char *buffer, int clientSocket);
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
    Vec<Client> &getClients();
    int checkIncomingClientConnection();
    bool isAllowedToConnect(string clientAddress, int socket);
    bool isAllowedToMakeRequest(Client *client);
    Channel *isInChannel(Client *client, string &channelName) const;

    Channel *addToChannel(Client *client, string &channelName);
    Channel *addToChannel(Client *client, string &channelName, string &key);
    void addChannel(Channel *newChannel);

    string hashPassword(const std::string &password);
    string getChannelId(const string &channelName, const string &channelKey);
    const Vec<Channel> getChannels() const;
    Channel *join(Client *client, std::string &channel);
    Channel *join(Client *client, std::string &channel, string &key);
    bool disconnect(Client *client);
    Vec<Channel> getClientChannels(Client *client);
    Vec<Channel> getClientInviteList(Client *client);
    Vec<Channel> getClientChannels(std::string &username);
    Vec<Client> getClientsInAChannel(Channel *channel);
    Client *getClient(std::string &username);
    bool isChannelFull(string &channelName);
    bool isValidPassword(Client *client);
    bool isAuthenticated(Client *client);
    bool setClientPassword(Client *client, const string &rawClientPassword);
    bool isModerator(Client *client, const string &channelName);
    bool isInChannel(Client *client, std::string &channel);
    bool channelExist(std::string &channel);
    bool hasTopic(std::string &channelName);
    bool removeClient(string &username);
    bool removeClient(Client *client);

    bool nickNameExist(string &ncikName);
    bool userNameExist(string &userName);
    string getChannelKey(std::string &channelName);
    bool channelKeyExist(std::string &channelName, std::string &key);
    Channel *getChannel(const string &channelName);
    Channel *removeClientFromChannel(Client *client, const std::string &channelName);
    Channel *kickClientFromChannel(Client *client, std::string &channelName);
    Channel *addClientToChannel(Client *client, std::string &channelName);
    Channel *addTopicToChannel(std::string &topic, std::string &channelName);
    Channel *removeTopicFromChannel(std::string &channelName);
    void removeChannel(Channel *channel);
    Client *getClientByNickname(std::string &nickname);
};