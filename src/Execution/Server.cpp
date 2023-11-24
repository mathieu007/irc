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
    return;
}

Server::Server(const Server &serv) : _max_fd_set(serv._max_fd_set), _pass(serv._pass), _port(serv._port), _serv_addr(serv._serv_addr), _client_adrr(serv._client_adrr), _client_size(serv._client_size)
{
}

Server::~Server(void)
{
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

const std::vector<Channel *> Server::getChannels() const
{
    std::map<string, Channel *>::const_iterator start = _channels.begin();
    std::map<string, Channel *>::const_iterator end = _channels.end();
    vector<Channel *> vec = vector<Channel *>();
    for (std::map<string, Channel *>::const_iterator it = start; it != end; ++it)
    {
        vec.push_back(it->second);
    }
    return vec;
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
        close(_socket);
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

bool Server::isAllowedToConnect(string clientAddress)
{
    Client *client;
    if (_bannedClients.tryGet(clientAddress, client))
    {
        if (client->isBannned() || !client->canConnect())
            return false;
        return true;
    }
    return true;
}

Client *Server::createOrGetClient(string clientAddress)
{
    Client *client = nullptr;
    if (!_bannedClients.tryGet(clientAddress, client))
        client = new Client();
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
    if (_clients.capacity() <= socketClient || _clients[socketClient] != NULL)
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
    client->setNickname("guestsdadsadsadsadsa");
    _clients.insert(_clients.begin() + socketClient, client);
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

    int socketClient = 0;
    for (int i = 0; i < FD_SETSIZE; i++)
    {
        std::string msg;
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
                sendMsg(_clients[socketClient], welcomeMsg, 0);
            }
            else
            {
                canMakeRequest = _clients[i]->canMakeRequest();
                if (canMakeRequest)
                    msg = recvMsg(_clients[i]->getSocket(), buffer, 0);
                else if (_clients[i]->isGoingToGetBanned())
                {
                    std::cerr << "The following client have been banned from our irc server: " << _clients[i]->getHost() << std::endl;
                    string key = _clients[i]->getHost();
                    _bannedClients.addOrReplace(key, _clients[i]);
                    string warning = string("You are now banned from our irc server, you have been warnned!");
                    sendMsg(_clients[i], warning, 0);
                    _clients[i] == nullptr;
                }
            }
        }
        if (!canMakeRequest)
        {
            std::cerr << "A client might be trying to ddos our irc server: " << _clients[i]->getHost() << std::endl;
            string warning = string("Trying to flood our irc server will get you banned forever!");
            sendMsg(_clients[i], warning, 0);
            continue;
        }
        _writing = _use;
        bool clearBuffer = msg.length() == 0;
        if (FD_ISSET(i, &_writing) && _clients[i])
        {
            if (!_clients[i]->getMsgQueue().empty())
                sendQueuedMsg(_clients[i], 0);
            else if (msg.length() > 0)
                clearBuffer = parseAndExec(_clients[i], msg, *this);
        }
        if (clearBuffer)
            msg.clear();
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
    close(_socket);
    closeFds();
    cleanAll();
}

bool Server::cleanAll()
{
    for (int i = 0; i < FD_SETSIZE; i++)
        delete _clients[i];
    Map<string, Channel *>::iterator it = _channels.begin();
    while (it != _channels.end())
    {
        if ((*it).second)
            delete (*it).second;
        it++;
    }
    return false;
}

bool Server::userNameExist(string &userName)
{
    vector<Client *>::const_iterator begin = this->_clients.begin();
    vector<Client *>::const_iterator end = this->_clients.end();
    while (begin != end)
    {
        if (*begin && userName == (*begin)->getUsername())
            return true;
        begin++;
    }
    return false;
}

void Server::closeFds(void)
{
    for (int i = 0; i < FD_SETSIZE; i++)
        if (FD_ISSET(i, &_use))
            close(i);
}

string Server::getChannelId(const string &channelName, const string &channelKey)
{
    string key = channelName + ":" + channelKey;
    return key;
}

bool Server::isAuthenticated(Client *client)
{
    return client->isAuthenticated();
}

bool Server::checkAndSetAuthorization(Client *client, const string &rawClientPassword)
{
    if (client->isAuthenticated())
        return true;
    if (_pass == hashPassword(rawClientPassword))
    {
        client->setIsAuthorized(true);
        return true;
    }
    return false;
}

bool Server::isModerator(Client *client, const string &channelName)
{
    Channel *channel = getChannel(channelName);
    if (!channel)
        return false;
    vector<Client *> moderators = channel->getModerators();
    string username = client->getUsername();
    if (Vector::isIn(moderators, &Client::getUsername, username))
        return true;
    return false;
}
bool Server::isInChannel(Client *client, string &channelName)
{
    Channel *channel = nullptr;
    if (_channels.tryGet(channelName, channel) && channel && client->isInChannel(channel))
        return true;
    return false;
}
bool Server::isInChannel(Client *client, string &channelName, string &key)
{
    Channel *channel = nullptr;
    if (!client)
        return false;
    string id = getChannelId(channelName, key);
    if (_channels.tryGet(id, channel) && channel && client->isInChannel(channel))
        return true;
    return false;
}

bool Server::isInKickChannel(Client *client, std::string &channelName)
{
    Channel *channel = nullptr;
    if (_channels.tryGet(channelName, channel) && channel && client->isInKickChannel(channel))
        return true;
    return false;
}

bool Server::isInKickChannel(Client *client, std::string &channelName, std::string &key)
{
    Channel *channel = nullptr;
    if (!client)
        return false;
    string id = getChannelId(channelName, key);
    if (_channels.tryGet(id, channel) && channel && client->isInKickChannel(channel))
        return true;
    return false;
}

bool Server::channelExist(string &channel)
{
    return _channels.hasKey(channel);
}

bool Server::channelExist(string &channel, string &key)
{
    string id = getChannelId(channel, key);
    return _channels.hasKey(id);
}

bool Server::hasTopic(string &channelName)
{
    Channel *channel = nullptr;
    if (_channels.tryGet(channelName, channel) && channel && channel->hasTopic())
        return true;
    return false;
}

bool Server::hasTopic(string &channelName, string &key)
{
    Channel *channel = nullptr;
    string id = getChannelId(channelName, key);
    if (_channels.tryGet(id, channel) && channel && channel->hasTopic())
        return true;
    return false;
}

vector<Client *> Server::getClientsInAChannel(Channel *channel)
{
    vector<Client *> clients = vector<Client *>();
    if (!channel)
        return clients;
    vector<Client *>::iterator it = _clients.begin();
    while (it != _clients.end())
    {
        if (*it && (*it)->isInChannel(channel))
        {
            clients.push_back(*it);
        }
        it++;
    }
    return clients;
}

Channel *Server::getChannel(const string &channelName)
{
    Channel *channel = nullptr;
    cout << "map Count " << _channels.size() << std::endl;
    if (_channels.tryGet(channelName, channel) && channel)
        return channel;
    return nullptr;
}

Channel *Server::getChannel(const std::string &channelName, const std::string &key)
{
    Channel *channel = nullptr;
    string id = getChannelId(channelName, key);
    if (_channels.tryGet(id, channel) && channel)
        return channel;
    return nullptr;
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
    vector<Channel *> channels = getClientChannels(client);
    vector<Channel *>::iterator it = channels.begin();
    while (it != channels.end())
    {
        if (*it)
        {
            if (!(*it)->getKey().empty())
                removeClientFromChannel(client, (*it)->getName(), (*it)->getKey());
            else
                removeClientFromChannel(client, (*it)->getName());
        }
        it++;
    }
    return true;
}

Channel *Server::removeClientFromChannel(Client *client, const std::string &channelName)
{
    Channel *channel = nullptr;
    if (_channels.tryGet(channelName, channel) && channel && client->getChannels().remove(channel->getId()))
        return channel;
    return nullptr;
}

Channel *Server::removeClientFromChannel(Client *client, const std::string &channelName, const std::string &key)
{
    Channel *channel = nullptr;
    string id = getChannelId(channelName, key);
    if (_channels.tryGet(id, channel) && channel && client->getChannels().remove(channel->getId()))
        return channel;
    return nullptr;
}

Channel *Server::kickClientFromChannel(Client *client, std::string &channelName)
{
    Channel *channel = nullptr;
    if (_channels.tryGet(channelName, channel) && channel && client->addToKickedChannel(channel))
        return channel;
    return nullptr;
}

Channel *Server::kickClientFromChannel(Client *client, std::string &channelName, std::string &key)
{
    Channel *channel = nullptr;
    string id = getChannelId(channelName, key);
    if (_channels.tryGet(id, channel) && channel && client->addToKickedChannel(channel))
        return channel;
    return nullptr;
}

bool Server::banClient(Client *client)
{
    if (_bannedClients.hasKey(client->getUsername()))
        return false;
    _bannedClients.add(client->getUsername(), client);
    if (_clients.size() < client->getSocket())
        return true;
    _clients[client->getSocket()] = NULL;
    _clients.erase(_clients.begin() + client->getSocket());
    return true;
}

Channel *Server::addClientToChannel(Client *client, std::string &channelName)
{
    Channel *channel = getChannel(channelName);
    if (channel && client->addToChannel(channel))
        return channel;
    return nullptr;
}

Channel *Server::addClientToChannel(Client *client, std::string &channelName, std::string &key)
{
    Channel *channel = getChannel(channelName, key);
    if (channel && client->addToChannel(channel))
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

Channel *Server::addTopicToChannel(std::string &topic, std::string &channelName, std::string &key)
{
    Channel *channel = getChannel(channelName, key);
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
    if (channel->hasTopic())
    {
        string topic = "";
        channel->setTopic(topic);
        return channel;
    }
    return nullptr;
}

Channel *Server::removeTopicFromChannel(std::string &channelName, std::string &key)
{
    Channel *channel = getChannel(channelName);
    if (channel->hasTopic())
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
vector<Client *> Server::removeChannel(std::string &channelName)
{
    Channel *channel = getChannel(channelName);
    vector<Client *> vec = getClientsInAChannel(channel);
    vector<Client *>::iterator it = vec.begin();
    while (it != vec.end())
    {
        if (*it)
            (*it)->removeFromChannel(channel);
        it++;
    }
    return vec;
}

vector<Client *> Server::removeChannel(std::string &channelName, std::string &key)
{
    Channel *channel = getChannel(channelName, key);
    vector<Client *> vec = getClientsInAChannel(channel);
    vector<Client *>::iterator it = vec.begin();
    while (it != vec.end())
    {
        if (*it)
            (*it)->removeFromChannel(channel);
        it++;
    }
    return vec;
}

Channel *Server::join(Client *client, std::string &channelName)
{
    Channel *channel = getChannel(channelName);
    if (!channel)
    {
        channel = new Channel(channelName);
        channel->setSuperModerator(client);
    }
    _channels.addIfNotExist(channel->getId(), channel);
    client->addToChannel(channel);
    return channel;
}

Channel *Server::join(Client *client, std::string &channelName, std::string &key)
{
    Channel *channel = getChannel(channelName, key);
    if (!channel)
    {
        channel = new Channel(channelName, key);
        channel->setSuperModerator(client);
    }
    _channels.addIfNotExist(channel->getId(), channel);
    client->addToChannel(channel);
    return channel;
}

bool Server::disconnect(Client *client)
{
    if (!client)
        return false;
    return true;
}

vector<Channel *> Server::getClientChannels(Client *client)
{
    vector<Channel *> vec = vector<Channel *>();
    Map<string, Channel *> channels = client->getChannels();
    Map<string, Channel *>::iterator it = channels.begin();
    while (it != channels.end())
    {
        if ((*it).second)
            vec.push_back((*it).second);
        it++;
    }
    return vec;
}

vector<Channel *> Server::getClientChannels(std::string &username)
{
    vector<Channel *> vec = vector<Channel *>();
    Client *client = getClient(username);
    if (!client)
        return vec;
    Map<string, Channel *> channels = client->getChannels();
    Map<string, Channel *>::iterator it = channels.begin();
    while (it != channels.end())
    {
        if ((*it).second)
            vec.push_back((*it).second);
        it++;
    }
    return vec;
}

Client *Server::getClient(std::string &username)
{
    vector<Client *>::const_iterator begin = this->_clients.begin();
    vector<Client *>::const_iterator end = this->_clients.end();
    while (begin != end)
    {
        if (*begin && username == (*begin)->getUsername())
            return (*begin);
        begin++;
    }
    return nullptr;
}

Client *Server::getClientByNickname(std::string &nickname)
{
    vector<Client *>::const_iterator begin = this->_clients.begin();
    vector<Client *>::const_iterator end = this->_clients.end();
    while (begin != end)
    {
        if (*begin && nickname == (*begin)->getNickname())
            return (*begin);
        begin++;
    }
    return nullptr;
}

bool Server::nickNameExist(std::string &nickname)
{
    vector<Client *>::const_iterator begin = this->_clients.begin();
    vector<Client *>::const_iterator end = this->_clients.end();
    while (begin != end)
    {
        if (*begin && nickname == (*begin)->getNickname())
            return true;
        begin++;
    }
    return false;
}
