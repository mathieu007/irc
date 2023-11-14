#ifndef IRC_HPP
#define IRC_HPP

#include <map>
#include <list>
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


class Server
{
private:
    std::string _pass;
    int _port;
    struct sockaddr_in _s_addr_in;
    socklen_t _cli_size;
    int _socket;
    struct timeval _tv;
    fd_set use;
    fd_set writing;
    fd_set reading;

public:
    Server(char *pass, int port);
    ~Server();

    Server(const Server &serv);
    Server &operator=(const Server &serv);

    void setSocket(int socket);

    int getPort() const;
    std::string getPass() const;
    int getSocket() const;
    sockaddr_in getSockAddr() const;
    socklen_t getCliSize() const;

    bool initServ();
    void launchServ();
    void closeServ();
    int acceptClients(char *host, char *service);

    void connectNewUser(int, fd_set &use, char *host);
    void findCmd(int index, fd_set &use);

};

#endif