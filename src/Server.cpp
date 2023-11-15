#include "Server.hpp"

Server::Server(char *pass, int port) : _pass(pass), _port(port)
{
    this->_hostname = _getHostname();
    _serv_adrr.sin_family = AF_INET;
    _serv_adrr.sin_port = htons(this->_port);
    _serv_adrr.sin_addr.s_addr = INADDR_ANY;
    _serv_size = sizeof(this->_serv_adrr);
    _client_size = _serv_size;
    return;
}

Server::Server(const Server &serv) : _pass(serv._pass), _port(serv._port), _serv_adrr(serv._serv_adrr), _client_adrr(serv._client_adrr), _client_size(serv._client_size)
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
    this->_serv_adrr = serv._serv_adrr;
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

void Server::_initServerSocket(void)
{
    this->_socket = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM == TCP connection type, IPPROTO_TCP == TCP Protocol // if cannot connect change it to 0
    if (this->_socket < 0)
        throw std::runtime_error("Error: Couldn't open server socket.\n");
    int enable = 1;
    if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        throw std::runtime_error("Error: Couldn't reuse socket address.\n");
    if (bind(this->_socket, (sockaddr *)&(this->_serv_adrr), this->_serv_size) == -1)
    {        
        close(this->_socket);
        throw std::runtime_error("Error: Couldn't bind socket address.\n");
    }
    if (listen(this->_socket, SOMAXCONN) == -1)
        throw std::runtime_error("Error: Cannot listen to socket connection.");
}

int Server::_setSockAddrStorage()
{
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);
    int result = getnameinfo((struct sockaddr *)&_serv_adrr, sizeof(_serv_adrr),
                             host, NI_MAXHOST,
                             service, NI_MAXSERV,
                             NI_NUMERICHOST | NI_NUMERICSERV);

    if (result == 0)
    {
        std::cout << "New connection established to Host: " << host << " Port: " << service << std::endl;
        _sock_host = string(host);
        _sock_port = string(service);
        _serv_size = sizeof(_serv_adrr);
        return 0;
    }
    else
    {
        std::cerr << "getnameinfo failed: " << gai_strerror(result) << std::endl;
        return -1;
    }
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

bool end = false;

void sigNcHandler(int sig, siginfo_t *info, void *context)
{
    (void)info;
    (void)context;
    (void)sig;
    end = true;
}

void setSignal(void)
{
    struct sigaction sigNc;

    memset(&sigNc, 0, sizeof(sigNc));
    sigaddset(&sigNc.sa_mask, SIGKILL);
    sigNc.sa_sigaction = &sigNcHandler;
    sigaction(SIGINT, &sigNc, NULL);
}

void Server::addClient(std::map<int, Client *> &clients, int socketClient, fd_set &use)
{
    Client *client = new Client();
    client->setSocket(socketClient);
    client->setHost(_sock_host);
    clients[socketClient] = client;
    FD_SET(socketClient, &use);
}

string Server::readClientMsg(Client *client)
{
    std::string msg;
    char buffer[2048];

    while (true) 
    {
        int read = recv(client->getSocket(), buffer, sizeof(buffer), 0);
       
        if (read == -1)
            std::cerr << "Error reading from client socket." << std::endl;
        if (read == 0)
            break;
        buffer[read] = 0;
        msg += buffer;
    }
    return msg;
}

void sendMsg(int socketClient)
{
    std::string message = ":user NICK :math\r\n";
    send(socketClient, message.c_str(), message.size(), 0);
    std::cout << "sent:       " << message << std::endl;
    /// USER
    std::string messageu = "USER math 0 * :Ronnie Reagan\r\n";
    send(socketClient, message.c_str(), messageu.size(), 0);
    std::cout << "sent:       " << messageu << std::endl;
    /// JOIN
    std::string messagej = ":math JOIN #Twilight_zone \r\n";
    send(socketClient, messagej.c_str(), messagej.size(), 0);
    std::cout << "sent:       " << messagej << std::endl;
}

int Server::fdsClientMsgLoop()
{
    int socketClient = 0;
    string msg = std::string("");
    for (int i = 0; i < FD_SETSIZE; i++)
    {
        if (FD_ISSET(i, &_reading))
        {
            if (i == _socket)
            {
                socketClient = acceptClient(); // accept == accept connexions on a socket
                if (socketClient < 0)
                    return -1;   
                addClient(_clients, socketClient, _use);
            }
            else
                msg = readClientMsg(_clients[i]);
        }
        _writing = _use;
        if (FD_ISSET(i, &_writing) && msg.length() > 0)
            sendMsg(socketClient);
        // if ()
        // findCmd(Channels, Users, i, use);
    }
    return 1;
}

void Server::initServer(void)
{
    try {
        _initServerSocket();
    }
    catch (const std::exception &e) 
    {
        std::cerr << e.what() << std::endl;
        return;
    }
    FD_ZERO(&_use);
    FD_SET(_socket, &_use);
    setSignal();
    while (1)
    {       
        _writing = _use;
        _reading = _use;
        struct timeval timeout = {0, 0};
        if (select(FD_SETSIZE, &_reading, &_writing, NULL, &timeout) == -1)
        {
            std::cerr << "select() error" << std::endl;
            return;
        }
        if (end == true)
            closeServer();
        if (fdsClientMsgLoop() == -1)
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
