#include "Server.hpp"

Server::Server(char *pass, int port, char *ip, bool fileLog) : _pass(pass), _port(port)
{
    if (fileLog)
        _logger = Logger();
    this->_hostname = _getHostname();
    memset(&_serv_addr, 0, sizeof(_serv_addr));
    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_port = htons(this->_port);
    _serv_addr.sin_addr.s_addr = inet_addr(ip);
    _serv_size = sizeof(this->_serv_addr);
    _client_size = _serv_size;
    _max_fd_set = 0;
    return;
}

Server::Server(char *pass, int port, bool fileLog) : _pass(pass), _port(port)
{
    if (fileLog)
        _logger = Logger();
    this->_hostname = _getHostname();
    memset(&_serv_addr, 0, sizeof(_serv_addr));
    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_port = htons(this->_port);
    _serv_addr.sin_addr.s_addr = INADDR_ANY;
    _serv_size = sizeof(this->_serv_addr);
    _client_size = _serv_size;
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

const std::vector<IChannel *> &Server::getChannels() const
{
    return _channels;
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
        std::cerr << "Failed to connect client to socket." << std::endl;
        close(socketClient);
        // close(_socket);
        return (-1);
    }
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

bool Server::isAllowedToConnect(string clientAddress)
{
    std::map<string, Client *>::iterator banClient = _bannedClients.find(clientAddress);
    if (banClient != _bannedClients.end())
    {
        Client *bc = banClient->second;
        if (bc->isBannned() || !bc->canConnect())
            return false;
        return true;
    }
    return true;
}

Client *Server::createOrGetClient(string clientAddress)
{
    std::map<string, Client *>::iterator bc = _bannedClients.find(clientAddress);
    if (bc != _bannedClients.end())
        return bc->second;
    Client *client = new Client();
    _bannedClients[clientAddress] = client;
    return client;
}

int Server::addClient(int socketClient, fd_set &use)
{
    string clientAddress = string();
    string clientPort = string();
    int result = getAddress(_client_adrr, _client_size, clientAddress, clientPort);
    if (result != 0)
    {
        std::cerr << "Could not get client addresse and port." << std::endl;
        return -1;
    }
    if (_clients[socketClient] != NULL)
    {
        std::cerr << "Client could not be added at index: " << socketClient << std::endl;
        return -1;
    }
    if (!isAllowedToConnect(clientAddress))
        return -1;
    Client *client = createOrGetClient(clientAddress);
    if (!client->canMakeRequest())
        return -1;
    client->setAddress(clientAddress);
    client->setPort(clientPort);
    client->setSocket(socketClient);
    client->setNickname("guest");
    _clients[socketClient] = client;
    FD_SET(socketClient, &use);
    return 0;
}

string getWelcomeMsg(Client *client)
{
    std::string message;

    message += ":irc 001 ";
    message += client->getNickname() + " ";
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

int Server::fdSetClientMsgLoop(char *buffer)
{
    std::string msg;
    int socketClient = 0;
    for (int i = 0; i < FD_SETSIZE; i++)
    {

        bool canMakeRequest = true;
        if (FD_ISSET(i, &_reading))
        {
            if (i == _socket)
            {
                socketClient = acceptClient();
                if (socketClient < 0)
                    return -1;
                if (addClient(socketClient, _use) == -1)
                    continue;
                _setNonBlocking(socketClient);
                string welcomeMsg = getWelcomeMsg(_clients[socketClient]);
                nonBlockingSend(_clients[socketClient], welcomeMsg, 0);
            }
            else
            {
                canMakeRequest = _clients[i]->canMakeRequest();
                if (canMakeRequest)
                    msg = nonBlockingRecv(_clients[i]->getSocket(), buffer, 0);
                else if (_clients[i]->isGoingToGetBanned())
                {
                    std::cerr << "The following client have been banned from our irc server: " << _clients[i]->getHost() << std::endl;
                    _bannedClients[_clients[i]->getHost()] = _clients[i];
                    string warning = string("You are now banned from our irc server, you have been warnned!");
                    nonBlockingSend(_clients[i], warning, 0);
                }
            }
        }
        if (!canMakeRequest)
        {
            std::cerr << "A client might be trying to ddos our irc server: " << _clients[i]->getHost() << std::endl;
            string warning = string("Trying to flood our irc server will get you banned forever!");
            nonBlockingSend(_clients[i], warning, 0);
            continue;
        }
        _writing = _use;
        if (FD_ISSET(i, &_writing) && msg.length() > 0)
        {
            static CommandFactory factory;

            factory.tokenMessage(msg, _clients[i], *this);

            // std::cout << "send msg: " << msg << std::endl;
            // nonBlockingSend(_clients[i], msg, 0);
            // std::cout << msg << std::endl;
            // parseExec(_clients[i], msg, *this);
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

IChannel *Server::isInChannel(Client *client, string &channelName) const
{
    return client->isInChanel(channelName);
}

IChannel *Server::addToChannel(Client *client, string &channelName, string &key)
{
    std::map<string, string>::iterator keyChannelName = _channelKeys.find(key);
    if (keyChannelName != _channelKeys.end() && channelName != keyChannelName->second)
        return nullptr;
    return client->addToChanel(channelName);
}

IChannel *Server::addToChannel(Client *client, string &channelName)
{
    return client->addToChanel(channelName);
}

bool Server::userNameInUse(string &userName)
{
    vector<Client *>::const_iterator begin = this->_clients.begin();
    vector<Client *>::const_iterator end = this->_clients.end();
    while (begin != end)
    {
        if (userName == (*begin)->getUsername())
            return true;
        begin++;
    }
    return false;
}

void Server::closeServer(void)
{
    for (int i = 0; i < FD_SETSIZE; i++)
        if (FD_ISSET(i, &_use))
            close(i);
    exit(1);
}

IChannel *Server::doesChannelExist(const std::string &channelName) const
{
    for (std::vector<IChannel *>::const_iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        const Channel *channel = dynamic_cast<const Channel *>(*it);
        if (channel && channel->getChannelName() == channelName)
        {
            return *it; // Return the pointer to the existing channel
        }
    }
    return nullptr; // Channel with the given name does not exist
}

void Server::addChannel(IChannel *newChannel)
{
    // Check if the channel already exists
    if (std::find(_channels.begin(), _channels.end(), newChannel) == _channels.end())
    {
        // Channel doesn't exist, add it to the container
        _channels.push_back(newChannel);
    }
    else
    {
        // Channel already exists, handle accordingly (e.g., log an error or take appropriate action)
        // You might also want to delete the newChannel if not needed
        delete newChannel;
    }
}

bool Server::isModerator(Client *client)
{
    // return true;
    return false;
}
bool Server::isInChannel(Client *client, string &channel)
{
    // return true;
    return false;
}
bool Server::isInChannel(string &username, string &channel)
{
    // return true;
    return false;
}
bool Server::isInChannel(Client *client, string &channel, string &key)
{
    // return true;
    return false;
}
bool Server::channelExist(string &channel)
{
    // return true;
    return false;
}
bool Server::channelExist(string &channel, string &key)
{
    // return true;
    return false;
}
bool Server::hasTopic(string &channelName)
{
    // return true;
    return false;
}
IChannel *Server::getChannel(std::string &channelName)
{
    // Implement actual logic here
    // return new Channel(channelName);
    return nullptr;
}

IChannel *Server::getChannel(std::string &channelName, std::string &key)
{
    // Implement actual logic here
    // return new Channel(channelName);
    return nullptr;
}

IChannel *Server::isValidChannelKey(std::string &channelName, std::string &key)
{
    // Implement actual logic here
    // return new Channel(channelName);
    return nullptr;
}

IChannel *Server::removeClientFromChannel(Client *client, std::string &channelName)
{
    // Implement actual logic here
    // return new Channel(channelName);
    return nullptr;
}

IChannel *Server::removeClientFromChannel(Client *client, std::string &channelName, std::string &key)
{
    // Implement actual logic here
    // return new Channel(channelName);
    return nullptr;
}

IChannel *Server::removeClientFromChannel(std::string &username, std::string &channelName)
{
    // Implement actual logic here
    // return new Channel(channelName);
    return nullptr;
}

IChannel *Server::removeClientFromChannel(std::string &username, std::string &channelName, std::string &key)
{
    // Implement actual logic here
    // return new Channel(channelName);
    return nullptr;
}

IChannel *Server::banClientFromChannel(Client *client, std::string &channelName)
{
    // Implement actual logic here
    // return new Channel(channelName);
    return nullptr;
}

IChannel *Server::banClientFromChannel(Client *client, std::string &channelName, std::string &key)
{
    // Implement actual logic here
    // return new Channel(channelName);
    return nullptr;
}

IChannel *Server::addClientToChannel(Client *client, std::string &channelName)
{
    // Implement actual logic here
    // return new Channel(channelName);
    return nullptr;
}

IChannel *Server::addClientToChannel(Client *client, std::string &channelName, std::string &key)
{
    // Implement actual logic here
    // return new Channel(channelName);
    return nullptr;
}

IChannel *Server::addTopicToChannel(std::string &topic, std::string &channelName)
{
    // Implement actual logic here
    // return new Channel(channelName);
    return nullptr;
}

IChannel *Server::addTopicToChannel(std::string &topic, std::string &channelName, std::string &key)
{
    // Implement actual logic here
    // return new Channel(channelName);
    return nullptr;
}

IChannel *Server::removeTopicFromChannel(std::string &channelName)
{
    // Implement actual logic here
    // return new Channel(channelName);
    return nullptr;
}

IChannel *Server::removeTopicFromChannel(std::string &channelName, std::string &key)
{
    // Implement actual logic here
    // return new Channel(channelName);
    return nullptr;
}

IChannel *Server::removeChannel(std::string &channelName)
{
    // Implement actual logic here
    // return new Channel(channelName);
    return nullptr;
}

IChannel *Server::removeChannel(std::string &channelName, std::string &key)
{
    // Implement actual logic here
    // return new Channel(channelName);
    return nullptr;
}

IChannel *Server::join(Client *client, std::string &channel)
{
    // Implement actual logic here
    // return new Channel(channel);
    return nullptr;
}

IChannel *Server::join(Client *client, std::string &channel, std::string &key)
{
    // Implement actual logic here
    // return new Channel(channel);
    return nullptr;
}

std::vector<IChannel *> Server::getClientChannels(Client *client)
{
    // Implement actual logic here
    // return std::vector<Channel *>();
    return std::vector<IChannel *>(); // return an empty vector
}

std::vector<IChannel *> Server::getClientChannels(std::string &username)
{
    // Implement actual logic here
    return std::vector<IChannel *>(); // return an empty vector
}
Client *Server::getClient(std::string &username)
{
    // Implement actual logic here
    // return nullptr;
    return nullptr; // replace nullptr with actual logic
}

bool Server::nickNameExist(std::string &nickName)
{
    return false;
}