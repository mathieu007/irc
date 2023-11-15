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
#include "Channel.hpp"
#include "Client.hpp"

class Server
{
private:
    std::string _hostname;
    std::string _pass;
    int _port;
    struct sockaddr_in _serv_adrr;
    socklen_t _serv_size;
    struct sockaddr_in _client_adrr;
    socklen_t _client_size;

    struct sockaddr_storage _sock_addr_storage;
    string _sock_port;
    string _sock_host;
    int _socket;

    /// @brief non blocking io, for monitoring both read and write operations.
    fd_set _use;
    /// @brief non blocking io for writing.
    fd_set _writing;
    /// @brief non blocking io for reading.
    fd_set _reading;

    std::map<int, Client *> _clients;
    vector<Channel *> _channels;

    int _setSockAddrStorage();
    string _getHostname() const;
    void _initServerSocket(void);

public:
    Server(char *pass, int port);
    ~Server();
    Server(const Server &serv);
    Server &operator=(const Server &serv);
    void findCmd(int index, fd_set &use);
    int acceptClient();
    void initServer(void);
    void closeServer(void);
    void addClient(std::map<int, Client *> &clients, int socketClient, fd_set &use);
    int fdsClientMsgLoop();
    string readClientMsg(Client *client);
};
