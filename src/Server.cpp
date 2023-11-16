#include "Server.hpp"

Server::Server(char *pass, int port, char *ip) : _pass(pass), _port(port)
{
    this->_hostname = _getHostname();
    memset(&_serv_addr, 0, sizeof(_serv_addr));
    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_port = htons(this->_port);
    _serv_addr.sin_addr.s_addr = inet_addr(ip);
    _serv_size = sizeof(this->_serv_addr);
    _client_size = _serv_size;
    std::string addr = string();
    std::string portstr = string();
    _max_fd_set = 0;
    return;
}

Server::Server(char *pass, int port) : _pass(pass), _port(port)
{
    this->_hostname = _getHostname();
    memset(&_serv_addr, 0, sizeof(_serv_addr));
    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_port = htons(this->_port);
    _serv_addr.sin_addr.s_addr = INADDR_ANY;
    _serv_size = sizeof(this->_serv_addr);
    _client_size = _serv_size;
    std::string addr = string();
    std::string portstr = string();
    _max_fd_set = 0;
    return;
}

Server::Server(const Server &serv) : _max_fd_set(serv._max_fd_set), _pass(serv._pass), _port(serv._port), _serv_addr(serv._serv_addr), _client_adrr(serv._client_adrr), _client_size(serv._client_size)
{
}

Server::~Server(void)
{
    return;
}

Server &Server::operator=(const Server &serv)
{
    this->_pass = serv._pass;
    this->_port = serv._port;
    this->_hostname = serv._hostname;
    this->_serv_addr = serv._serv_addr;
    this->_serv_size = serv._serv_size;
    this->_client_size = serv._client_size;
    this->_client_adrr = serv._client_adrr;
    return (*this);
}

string Server::_getHostname() const
{
    char hostname[256];

    if (gethostname(hostname, sizeof(hostname)) == 0)
    {
        return string(hostname);
    }
    else
    {
        throw std::runtime_error("Could not get hostname.\n");
        return NULL;
    }
}

int Server::getServerIp(string &ip)
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        perror("gethostname");
        return -1;
    }
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, NULL, &hints, &res) != 0)
    {
        perror("getaddrinfo");
        return -1;
    }

    for (struct addrinfo *p = res; p != NULL; p = p->ai_next)
    {
        void *addr;
        if (p->ai_family == AF_INET)
        {
            struct sockaddr_in *ipv4 = reinterpret_cast<struct sockaddr_in *>(p->ai_addr);
            addr = &(ipv4->sin_addr);
        }
        else
        {
            struct sockaddr_in6 *ipv6 = reinterpret_cast<struct sockaddr_in6 *>(p->ai_addr);
            addr = &(ipv6->sin6_addr);
        }
        char ipstr[INET6_ADDRSTRLEN];
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        ip.append(ipstr);
    }
    freeaddrinfo(res);
    return 0;
}

void Server::_initServerSocket(void)
{
    this->_socket = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM == TCP connection type, IPPROTO_TCP == TCP Protocol // if cannot connect change it to 0
    if (this->_socket < 0)
        throw std::runtime_error("Error: Couldn't open server socket.\n");
    int enable = 1;
    if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        throw std::runtime_error("Error: Couldn't reuse socket address.\n");
    if (bind(this->_socket, (sockaddr *)&(this->_serv_addr), this->_serv_size) == -1)
    {
        close(this->_socket);
        throw std::runtime_error("Error: Couldn't bind socket address.\n");
    }
    if (listen(this->_socket, SOMAXCONN) == -1)
        throw std::runtime_error("Error: Cannot listen to socket connection.\n");
}

int Server::_setSockAddrStorage()
{
    string addr = string();
    string port = string();
    int result = getAddress(_serv_addr, _serv_size, addr, port);
    if (result == 0)
    {
        _sock_host = addr;
        _sock_port = port;
        return 0;
    }
    return result;
}

int Server::getAddress(sockaddr_in &sock_addr, socklen_t &size, string &address, string &port)
{
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);
    int result = getnameinfo(reinterpret_cast<struct sockaddr *>(&sock_addr), size,
                             host, NI_MAXHOST,
                             service, NI_MAXSERV,
                             NI_NUMERICHOST | NI_NUMERICSERV);
    if (result == 0)
    {
        std::cout << "New connection established to: " << host << ":" << service << std::endl;
        address.append(host);
        port.append(service);
        return 0;
    }
    else
        std::cerr << "getnameinfo failed: " << gai_strerror(result) << std::endl;

    return result;
}

int Server::acceptClient()
{
    int socketClient = accept(_socket, (sockaddr *)&_client_adrr, &_client_size); // accept == accept connexions on a socket
    if (socketClient < 0)
    {
        std::cerr << "Failed to client connection to socket." << std::endl;
        close(socketClient);
        close(_socket);
        return (-1);
    }
    if (_setSockAddrStorage() == -1)
        return -1;
    return (socketClient);
}

bool endSignal = false;

void sigNcHandler(int sig, siginfo_t *info, void *context)
{
    (void)info;
    (void)context;
    (void)sig;
    endSignal = true;
}

void setSignal(void)
{
    struct sigaction sigNc;

    memset(&sigNc, 0, sizeof(sigNc));
    sigaddset(&sigNc.sa_mask, SIGKILL);
    sigNc.sa_sigaction = &sigNcHandler;
    sigaction(SIGINT, &sigNc, NULL);
}

int Server::addClient(std::map<int, Client *> &clients, int socketClient, fd_set &use)
{
    std::string msg;
    string clientAddress = string();
    string clientPort = string();

    Client *client = new Client();
    int result = getAddress(_client_adrr, _client_size, clientAddress, clientPort);
    if (result != 0)
    {
        std::cerr << "Could not get client addresse and port." << std::endl;
        return -1;
    }
    client->setAddress(clientAddress);
    client->setPort(clientPort);
    client->setSocket(socketClient);
    clients[socketClient] = client;  
    FD_SET(socketClient, &use);
    return 0;
}

string getWelcomeMsg()
{
    std::string message;

    message += ":irc 001 ";
    message += "math ";
    message += "Welcome to our IRC.\r\n";
    return message;
}

void Server::_setNonBlocking(int sockfd)
{
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl");
        return;
    }
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("fcntl");
    }
}

ssize_t Server::nonBlockingSend(Client *client, string &data, int flags)
{
    ssize_t bytesSent = 0;
    // if a previous io operation failed or block, we need to get the leftover bytes from the client, it's how non blocking io operations should be done...
    std::string msg = client->getMsg();
    msg.append(data);
    const char *ptr = msg.c_str();
    while (bytesSent < (ssize_t)msg.length())
    {
        ptr = ptr + bytesSent;
        ssize_t result = send(client->getSocket(), ptr, msg.length() - bytesSent, flags);
        // the data is not sent in one swoop we must handle the data per client, we need to save the left over msg data into user msg
        if (result > 0)
            bytesSent += result;
        else if (result == 0)
        {
            client->setMsg(ptr);
            break;
        }
        else
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                client->setMsg(ptr);
                continue;
            }
            else if (errno == EINTR)
            {
                // Interrupted by a signal, continue sending ????????????
                continue;
            }
            else
            {
                // Error occurred
                perror("send");
                break;
            }
        }
    }
    client->getMsg().clear();
    return bytesSent;
}

string Server::_nonBlockingRecv(int sockfd, char *buffer, int flags)
{
    std::string msg = std::string();
    ssize_t bytesRead = 0;
    while (true)
    {
        bytesRead = recv(sockfd, buffer, MAX_BUFFER_SIZE, flags);
        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0';
            msg += buffer;
            return msg;
        }
        // Connection closed by the peer
        else if (bytesRead == 0)
            return msg;
        else
        {
            // iff no data available we don't wait and send the msg right away...
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return msg;
            }
            else if (errno == EINTR)
            {
                // should we continue ??????????
                break;
            }
            else
            {
                // Error occurred
                perror("recv");
                break;
            }
        }
    }
    return msg;
}

int Server::fdSetClientMsgLoop(char *buffer)
{
    std::string msg;
    int socketClient = 0;
    for (int i = 0; i < FD_SETSIZE; i++)
    {
        if (FD_ISSET(i, &_reading))
        {
            if (i == _socket)
            {
                socketClient = acceptClient();
                if (socketClient < 0)
                    return -1;
                _setNonBlocking(socketClient);
                if (addClient(_clients, socketClient, _use) == -1)
                    continue;
                string welcomeMsg = getWelcomeMsg();
                nonBlockingSend(_clients[socketClient], welcomeMsg, 0);
            }
            else
                msg = _nonBlockingRecv(_clients[i]->getSocket(), buffer, 0);
        }
        _writing = _use;
        if (FD_ISSET(i, &_writing) && msg.length() > 0)
        {
            std::cout << "send msg: " << msg << std::endl;
            nonBlockingSend(_clients[i], msg, 0);
        }
    }
    return 1;
}

int Server::_selectFdSet()
{
    _writing = _use;
    _reading = _use;
    struct timeval timeout = {0, 0};
    int result = select(FD_SETSIZE, &_reading, &_writing, NULL, &timeout);
    if (result == -1)
    {
        std::cerr << "select() error: " << strerror(errno) << std::endl;
        return -1;
    }
    return result;
}

void Server::initServer(void)
{
    try
    {
        _initServerSocket();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return;
    }
    FD_ZERO(&_use);
    FD_SET(_socket, &_use);
    _setNonBlocking(_socket);
    setSignal();
    if (_setSockAddrStorage() == -1)
        return;
    char buffer[MAX_BUFFER_SIZE];
    while (1)
    {
        if (_selectFdSet() < 0)
            return;
        if (endSignal == true)
            closeServer();
        if (fdSetClientMsgLoop(buffer) == -1)
            return;
    }
    close(_socket);
}

void Server::closeServer(void)
{
    for (int i = 0; i < FD_SETSIZE; i++)
        if (FD_ISSET(i, &_use))
            close(i);
    exit(1);
}
