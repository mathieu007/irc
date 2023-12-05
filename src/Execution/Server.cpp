#include "Server.hpp"
#include "CommandFactory.hpp"
#include "Message.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include <csignal>
#include <iostream>
#include "Vector.hpp"

// volatile sig_atomic_t flag = 0;

// void handler(int signum) {
//     cout << "Signal: " << signum << " sent." << std::endl;
//     flag = 1;
// }

Server::Server() {}

Server::Server(char *pass, int port, char *ip, bool fileLog) : _pass(hashPassword(pass)), _port(port)
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
    for (size_t i = 0; i < _clients.size(); i++)
    {
        _clients[i] = nullptr;
    }
    return;
}

Server::Server(char *pass, int port, bool fileLog) : _pass(hashPassword(pass)), _port(port)
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
    for (size_t i = 0; i < _clients.size(); i++)
    {
        _clients[i] = nullptr;
    }
    return;
}

Server::Server(const Server &serv) : _max_fd_set(serv._max_fd_set), _pass(serv._pass), _port(serv._port), _serv_addr(serv._serv_addr), _client_adrr(serv._client_adrr), _client_size(serv._client_size)
{
}

Server::~Server(void)
{
    CleanServer();
}

void Server::CleanServer()
{
    cout << "Start cleaning server: " << std::endl;
    _clients.removeAll(true);
    _channels.removeAll(true);
    _clientschannelsMapping->removeAll(true);
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

// not secure but as cpp98 doesn't support cryptographic...
string Server::hashPassword(const std::string &password)
{
    if (password.empty())
        return password;
    std::ostringstream hashed_ss;
    for (size_t i = 0; i < password.size(); i++)
    {
        hashed_ss << std::hex << std::setw(2) << std::setfill('0') << (int)password[i];
    }
    return hashed_ss.str();
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
    this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM == TCP connection type, IPPROTO_TCP == TCP Protocol // if cannot connect change it to 0
    if (this->_serverSocket < 0)
        throw std::runtime_error("Error: Couldn't open server socket.\n");
    int enable = 1;
    if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        throw std::runtime_error("Error: Couldn't reuse socket address.\n");
    if (bind(this->_serverSocket, (sockaddr *)&(this->_serv_addr), this->_serv_size) == -1)
    {
        close(this->_serverSocket);
        throw std::runtime_error("Error: Couldn't bind socket address.\n");
    }
    if (listen(this->_serverSocket, SOMAXCONN) == -1)
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
    int socketClient = accept(_serverSocket, (sockaddr *)&_client_adrr, &_client_size); // accept == accept connexions on a socket
    if (socketClient < 0)
    {
        std::cerr << "Failed to connect client to socket." << std::endl;
        // close(socketClient);
        close(_serverSocket);
        return (-1);
    }
    return (socketClient);
}

volatile sig_atomic_t endSignal = 0;

void signalHandler(int sig, siginfo_t *info, void *context)
{
    cout << std::endl
         << "A Signal have been received sig: " << sig << std::endl;
    (void)info;
    (void)context;
    (void)sig;
    endSignal = 1;
}

void setSignal(void)
{
    struct sigaction sigAction;

    memset(&sigAction, 0, sizeof(sigAction));
    sigaddset(&sigAction.sa_mask, SIGKILL);
    sigAction.sa_sigaction = &signalHandler;

    sigaction(SIGINT, &sigAction, NULL);  // Ctrl+C
    sigaction(SIGTSTP, &sigAction, NULL); // Ctrl+Z
    sigaction(SIGQUIT, &sigAction, NULL); // Ctrl+\..
}

bool Server::isAllowedToConnect(string clientAddress, int socket)
{
    long curTime = static_cast<long>(time(NULL));
    long lastConnection = 0;
    if (_connectionsLog.tryGet(clientAddress, lastConnection) && lastConnection + MAX_CLIENT_CONNECTION_RETRY_TIME > curTime)
    {
        _connectionsLog.add(clientAddress, curTime);
        string notice = "NOTICE :Too many connections attemps. Plz try to connect to our server in a few seconds.\r\n";
        send(socket, notice.c_str(), notice.length(), 0);
        std::cout << "Connection to server cancelled, too many connections attemps from : " << clientAddress << std::endl;
        return false;
    }
    _connectionsLog.add(clientAddress, curTime);
    long bannedTime = 0;
    if (_bannedClients.tryGet(clientAddress, bannedTime))
    {
        if (bannedTime < time(NULL))
            return false;
        return true;
    }
    return true;
}

int Server::createClient(int socketClient)
{
    string clientAddress = string();
    string clientPort = string();
    int result = getAddress(_client_adrr, _client_size, clientAddress, clientPort);
    if (result != 0)
    {
        std::cerr << "Could not get client addresse and port." << std::endl;
        return -1;
    }
    if (_clients.capacity() <= (size_t)socketClient || _clients[socketClient] != NULL)
    {
        std::cerr << "Client could not be added at index: " << socketClient << std::endl;
        return -1;
    }
    if (!isAllowedToConnect(clientAddress, socketClient))
        return -1;
    long nextConnection = 0;
    if (_bannedClients.tryGet(clientAddress, nextConnection))
    {
        _bannedClients.add(clientAddress, time(NULL) + NEXT_ALLOWED_CONNECTION_TIME_ONCE_BAN);
        return -1;
    }
    Client *client = new Client(_clientschannelsMapping);
    client->setAddress(clientAddress);
    client->setHost(clientAddress);
    client->setPort(clientPort);
    client->setSocket(socketClient);
    client->setNickname("guest");
    client->setLastActivity(static_cast<long>(time(NULL)));
    _clients[socketClient] = client;
    cout << "New client connection, address: " << clientAddress << ", socket: " << socketClient << std::endl;
    // FD_SET(socketClient, &use);
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

int Server::checkIncomingClientConnection()
{
    int socketClient = 0;
    if (FD_ISSET(_serverSocket, &_reading))
    {
        cout << "Incomming connection..." << std::endl;
        socketClient = acceptClient();
        if (socketClient < 0)
            return -1;
        if (createClient(socketClient) == -1)
        {
            close(socketClient);
            return 0;
        }
        _setNonBlocking(socketClient);
        _clientSockets.push_back(socketClient);
        Client *client = _clients[socketClient];
        string welcomeMsg = getWelcomeMsg(client);
        Msg::sendMsg(client, welcomeMsg, 0);
        cout << "Welcome message sent." << std::endl;
    }
    return socketClient;
}

bool Server::_recvClientMsg(Client *client, char *buffer, int index)
{
    int clientSocket = _clientSockets[index];
    string msg = Msg::recvMsg(clientSocket, buffer);
    if (msg.size() > 0)
    {
        client->getMsgRecvQueue().append(msg);
        client->setLastActivity(static_cast<long>(time(NULL)));
        if (client->getMsgRecvQueue().size() >= MAX_BUFFER_SIZE)
        {
            client->setMsgRecvQueue("");
            return false;
        }
    }
    return true;
}
void Server::_banClient(Client *client)
{
    std::cerr << "The following client: " + client->getHost() + " have been banned from our irc server." << std::endl;
    std::cout << "The following client: " + client->getHost() + " have been banned from our irc server." << std::endl;
    string key = client->getHost();
    _bannedClients.add(key, time(NULL) + NEXT_ALLOWED_CONNECTION_TIME_ONCE_BAN);
    string warning = string("QUIT :You are now banned from our irc server, you have been warnned several time!\r\n");
    Msg::sendMsg(client, warning, 0);
    removeClient(client);
}

void Server::_disconnectInnactiveClient(Client *client)
{
    string inactivityMsg = string("QUIT :You have been diconnected from our server, you've been inactive for too long.\r\n");
    Msg::sendMsg(client, inactivityMsg, 0);
    client->setRemove(true);
}

int Server::fdSetClientMsgLoop(char *buffer)
{
    checkIncomingClientConnection();
    int clientSocketCount = _clientSockets.size();
    for (int i = 0; i < clientSocketCount; i++)
    {
        int clientSocket = _clientSockets[i];
        Client *client = _clients[clientSocket];
        if (!client)
            continue;
        if (client->shouldBeRemoved())
        {
            removeClient(client);
            continue;
        }
        bool canMakeRequest = true;
        if (FD_ISSET(clientSocket, &_reading))
        {
            canMakeRequest = client->canMakeRequest();
            if (canMakeRequest)
            {
                if (!_recvClientMsg(client, buffer, i))
                {
                    _banClient(client);
                    break;
                }
            }
            else if (client->hasReachMaxReq())
            {
                _banClient(client);
                break;
            }
        }
        if (!canMakeRequest)
        {
            std::cerr << "A client might be trying to ddos our irc server: " << _clients[clientSocket]->getHost() << std::endl;
            string warning = string("NOTICE ");
            warning += _clients[clientSocket]->getNickname();
            warning += " Request canceled you have reach your maximum request per second!\r\n";
            warning += "You will be banned from our server if you continue...\r\n";
            Msg::sendMsg(_clients[clientSocket], warning, 0);
            continue;
        }
        if (FD_ISSET(clientSocket, &_writing) && _clients[clientSocket])
        {
            if (this->isAuthenticated(client))
            {
                if (!client->getMsgSendQueue().empty())
                    Msg::sendQueuedMsg(client, 0);
                else if (!client->getMsgRecvQueue().empty())
                    Msg::parseAndExec(client, client->getMsgRecvQueue(), *this);
            }
            else if (!client->getMsgRecvQueue().empty())
                _execUnAuthenticatedCmd(client->getMsgRecvQueue(), client);
            if (static_cast<long>(time(NULL)) - client->getLastActivity() >= MAX_CLIENT_INACTIVITY)
                _disconnectInnactiveClient(client);
        }
    }
    return 1;
}

void Server::_execUnAuthenticatedCmd(string &msg, Client *client)
{
    if (String::startWith(msg, "CAP"))
    {
        Msg::parseAndExec(client, msg, *this);
        if (String::startWith(msg, "CAP"))
            Msg::sendAuthMessages(client);
    }
    else if (String::startWith(msg, "NICK"))
        Msg::parseAndExec(client, msg, *this);
    else if (String::startWith(msg, "USER"))
        Msg::parseAndExec(client, msg, *this);
    else if (String::startWith(msg, "PASS"))
        Msg::parseAndExec(client, msg, *this);
    else if (String::startWith(msg, "PING"))
        Msg::parseAndExec(client, msg, *this);
    else if (String::startWith(msg, "QUIT"))
        Msg::parseAndExec(client, msg, *this);
    else if (!msg.empty())
    {
        msg.clear();
        Msg::sendAuthMessages(client);
    }
}

int Server::_selectFdSet()
{
    FD_ZERO(&_reading);
    FD_ZERO(&_writing);
    FD_SET(_serverSocket, &_reading);
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    int socketCount = _clientSockets.size();
    for (int i = 0; i < socketCount; i++)
    {
        int clientSocket = _clientSockets[i];
        FD_SET(clientSocket, &_reading);
        FD_SET(clientSocket, &_writing);
    }
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
    FD_ZERO(&_reading);
    FD_ZERO(&_writing);
    FD_SET(_serverSocket, &_reading);
    _setNonBlocking(_serverSocket);
    setSignal();
    if (_setSockAddrStorage() == -1)
        return;
    char buffer[MAX_BUFFER_SIZE];
    while (true && !std::cin.fail() && !std::cin.eof())
    {
        if (_selectFdSet() < 0)
            break;
        if (endSignal == 1)
            break;
        if (fdSetClientMsgLoop(buffer) == -1)
            break;
    }
    cout << "Closing server." << std::endl;
    close(_serverSocket);
    closeFds();
}

// bool Server::cleanAll()
// {
//     for (int i = 0; i < FD_SETSIZE; i++)
//     {
//         if (_clients[i])
//         {
//             delete _clients[i];
//         }~
//     }
//     Map<string, Channel *>::iterator it = _channels.begin();
//     while (it != _channels.end())
//     {
//         if ((*it).second)
//             delete (*it).second;
//         it++;
//     }
//     return false;
// }

bool Server::userNameExist(string &userName)
{
    return getClient(userName) != nullptr;
}

void Server::closeFds(void)
{
    for (int i = 0; i < FD_SETSIZE; i++)
        if (FD_ISSET(i, &_reading))
            close(i);
    for (int i = 0; i < FD_SETSIZE; i++)
        if (FD_ISSET(i, &_writing))
            close(i);
}

bool Server::isChannelFull(string &channelName)
{
    Channel *channel = getChannel(channelName);
    if (!channel)
        return false;
    return channel->getNumClients() == channel->getMaxNumClients();
}

bool Server::isAuthenticated(Client *client)
{
    return client->isValidUserInfo() && this->isValidPassword(client);
}

bool Server::isValidPassword(Client *client)
{
    return client->getPass() == this->_pass;
}

bool Server::setClientPassword(Client *client, const string &rawClientPassword)
{
    string hashedPass = hashPassword(rawClientPassword);
    if (_pass == hashedPass)
    {
        client->setPass(hashedPass);
        return true;
    }
    return false;
}

vector<int> &Server::getClientSockets()
{
    return _clientSockets;
}

bool Server::isModerator(Client *client, const string &channelName)
{
    Channel *channel = getChannel(channelName);
    if (!channel || !client)
        return false;
    if (channel->getSuperModerator() == client)
        return true;
    ClientChannelMapping *map = channel->getMapping().first(&ClientChannelMapping::getClientUsername, client->getUsername());
    if (map && map->getIsModerator())
        return true;
    return false;
}

bool Server::isInChannel(Client *client, string &channelName)
{
    if (!client)
        return false;
    Channel *channel = getChannel(channelName);
    return (client->isInChannel(channel));
}

bool Server::channelExist(string &channelName)
{
    return _channels.exist(&Channel::getName, channelName);
}

bool Server::hasTopic(string &channelName)
{
    Channel *channel = getChannel(channelName);
    if (!channel)
        return false;
    return channel->hasTopic();
}

Vec<Channel> Server::getClientInviteList(Client *client)
{
    if (!client)
        return Vec<Channel>();
    return client->getInviteList();
}

Vec<Client> Server::getClientsInAChannel(Channel *channel)
{
    return channel->getMapping().select(&ClientChannelMapping::getClient);
}

Channel *Server::getChannel(const string &channelName)
{
    return _channels.first(&Channel::getName, channelName);
}

bool Server::removeClient(string &username)
{
    Client *client = getClient(username);
    return removeClient(client);
}

bool Server::removeClient(Client *client)
{
    if (!client)
        return false;
    int socketFd = client->getSocket();
    if (client)
    {
        std::vector<int>::iterator it = std::find(_clientSockets.begin(), _clientSockets.end(), socketFd);
        if (it != _clientSockets.end())
            _clientSockets.erase(it);
    }
    Vec<ClientChannelMapping> maps = _clientschannelsMapping->where(&ClientChannelMapping::getClientUsername, client->getUsername());
    if (maps.size() > 0)
    {
        for (size_t i = 0; i < maps.size(); i++)
        {
            Channel *chan = maps[i]->getChannel();
            chan->setNumClients(chan->getNumClients() - 1);
        }
    }
    _clientschannelsMapping->removeWhere(&ClientChannelMapping::getClientUsername, client->getUsername(), true);
    _clientschannelsMapping->removeWhere(&ClientChannelMapping::getChannelClientCount, (uint)0, true);
    _channels.removeWhere(&Channel::getNumClients, (uint)0, true);
    // Set client to be removed on next loop...
    client->setRemove(false);
    delete _clients[socketFd];
    _clients[socketFd] = nullptr;
    close(socketFd);
    return true;
}

Channel *Server::removeClientFromChannel(Client *client, const std::string &channelName)
{
    Channel *channel = getChannel(channelName);
    if (!client || !channel)
        return nullptr;
    ClientChannelMapping *map = client->getMapping().first(&ClientChannelMapping::getChannelName, channelName);
    if (map)
    {
        _clientschannelsMapping->remove(map, true);
        channel->setNumClients(channel->getNumClients() - 1);
        _clientschannelsMapping->removeWhere(&ClientChannelMapping::getChannelClientCount, (uint)0, true);
    }
    if (channel->getNumClients() == 0)
        _channels.remove(channel, true);
    return channel;
}

Channel *Server::kickClientFromChannel(Client *client, std::string &channelName)
{
    Channel *channel = getChannel(channelName);
    if (!channel || !client)
        return nullptr;
    client->addToKickedChannel(channel);
    channel->setNumClients(channel->getNumClients() - 1);
    return channel;
}

Channel *Server::addClientToChannel(Client *client, std::string &channelName)
{
    Channel *channel = getChannel(channelName);
    if (channel && client->addChannel(channel))
        return channel;
    return nullptr;
}

Channel *Server::addTopicToChannel(std::string &topic, std::string &channelName)
{
    Channel *channel = getChannel(channelName);
    if (channel)
    {
        channel->setTopic(topic);
        return channel;
    }
    return nullptr;
}

Channel *Server::removeTopicFromChannel(std::string &channelName)
{
    Channel *channel = getChannel(channelName);
    if (channel && channel->hasTopic())
    {
        string topic = "";
        channel->setTopic(topic);
        return channel;
    }
    return nullptr;
}

/// @brief this will remove the channel but also all users from this channels if they are in...
/// @param channelName
/// @return we return the list of client from which they were removed.
void Server::removeChannel(Channel *channel)
{
    if (!channel)
        return;
    _clientschannelsMapping->removeWhere(&ClientChannelMapping::getChannelName, channel->getName(), true);
    _channels.remove(channel, true);
}

Channel *Server::join(Client *client, std::string &channelName)
{
    Channel *channel = getChannel(channelName);
    if (!channel)
    {
        channel = new Channel(channelName, _clientschannelsMapping);
        channel->setSuperModerator(client);
        _channels.push_back(channel);
    }
    ClientChannelMapping *map = channel->getMapping().first(&ClientChannelMapping::getClientUsername, client->getUsername());
    if (!map && client->canJoinChannel(channel))
    {
        ClientChannelMapping *map = new ClientChannelMapping(client, channel);
        _clientschannelsMapping->push_back(map);
        channel->setNumClients(channel->getNumClients() + 1);
    }
    return channel;
}

Channel *Server::join(Client *client, string &channelName, string &key)
{
    Channel *channel = getChannel(channelName);
    if (!channel)
    {
        channel = new Channel(channelName, key, _clientschannelsMapping);
        channel->setSuperModerator(client);
        _channels.push_back(channel);
    }
    ClientChannelMapping *map = channel->getMapping().first(&ClientChannelMapping::getClientUsername, client->getUsername());
    if (!map && client->canJoinChannel(channel))
    {
        ClientChannelMapping *map = new ClientChannelMapping(client, channel);
        _clientschannelsMapping->push_back(map);
        channel->setNumClients(channel->getNumClients() + 1);
    }
    return channel;
}

string Server::getChannelKey(std::string &channelName)
{
    Channel *channel = getChannel(channelName);
    if (channel)
        return channel->getKey();
    return string();
}

bool Server::channelKeyExist(std::string &channelName, std::string &key)
{
    Channel *channel = getChannel(channelName);
    if (channel->getKey() == key)
        return true;
    return false;
}

bool Server::disconnect(Client *client)
{
    if (!client)
        return false;
    client->setRemove(true);
    return true;
}

Vec<Channel> Server::getClientChannels(Client *client)
{
    if (!client)
        return Vec<Channel>();
    return client->getMapping().select(&ClientChannelMapping::getChannel);
}

Vec<Channel> Server::getClientChannels(std::string &username)
{
    Client *client = getClient(username);
    if (!client)
        return Vec<Channel>();
    return client->getMapping().select(&ClientChannelMapping::getChannel);
}

Vec<Client> &Server::getClients()
{
    return _clients;
}

Client *Server::getClient(std::string &username)
{
    return _clients.first(&Client::getUsername, username);
}

Client *Server::getClientByNickname(std::string &nickname)
{
    return _clients.first(&Client::getNickname, nickname);
}

bool Server::nickNameExist(std::string &nickname)
{
    if (nickname == "guest")
        return true;
    return _clients.exist(&Client::getNickname, nickname);
}
