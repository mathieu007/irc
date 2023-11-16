#include "Server.hpp"
#include "Command.hpp"

Server::Server(char *pass, int port) : _pass(pass), _port(port)
{
    this->_hostname = _getHostname();
    _serv_adrr.sin_family = AF_INET;
    _serv_adrr.sin_port = htons(this->_port);
    _serv_adrr.sin_addr.s_addr = INADDR_ANY;
    _serv_size = sizeof(this->_serv_adrr);
    _client_size = _serv_size;
    _max_fd_set = 0;
    return;
}

Server::Server(const Server &serv) : _max_fd_set(serv._max_fd_set), _pass(serv._pass), _port(serv._port), _serv_adrr(serv._serv_adrr), _client_adrr(serv._client_adrr), _client_size(serv._client_size)
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
        throw std::runtime_error("Error: Cannot listen to socket connection.\n");
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

void Server::addClient(std::map<int, Client *> &clients, int socketClient, fd_set &use)
{
    std::string msg;

    Client *client = new Client();
    client->setSocket(socketClient);
    client->setHost(_sock_host);
    clients[socketClient] = client;
    FD_SET(socketClient, &use);
}

string Server::readClientMsg(Client *client)
{
    std::string msg = string("");
    char buffer[4096];

    int read = recv(client->getSocket(), buffer, sizeof(buffer), 0);
    if (read == -1)
        std::cerr << "Error reading from client socket." << std::endl;
    buffer[read] = 0;
    msg += buffer;
    return msg;
}

string getWelcomeMsg()
{
    std::string message;

    message += ":irc 001 ";
    message += "bozo ";
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
                socketClient = acceptClient(); // accept == accept connexions on a socket
                if (socketClient < 0)
                    return -1;
                _setNonBlocking(socketClient);
                addClient(_clients, socketClient, _use);
                string welcomeMsg = getWelcomeMsg();
                nonBlockingSend(_clients[socketClient], welcomeMsg, 0);
            }
            else
                msg = _nonBlockingRecv(_clients[i]->getSocket(), buffer, 0);
        }
        _writing = _use;
        if (FD_ISSET(i, &_writing) && msg.length() > 0)
        {
			
			/////////parse msg
			std::vector<std::string> _tokens;
			std::istringstream iss(msg);

			/////////print msg
			// std::cout << "msg receved:" << msg << std::endl;
			std::string token;
			while (std::getline(iss, token, ' '))
			{
				_tokens.push_back(token);
				// std::cout << "Token: " << token << std::endl;
			}

			////////////print token
			for (std::vector<std::string>::iterator it = _tokens.begin(); it != _tokens.end(); ++it)
			{
				// 	std::cout << "token:" << *it << std::endl;

				//////is nick find execute
				if (*it == "NICK")
				{
					CommandFactory factory;

					Command *nickCommand = factory.createCommand("NICK");
					if (nickCommand)
					{
						nickCommand->execute(_clients[i], _tokens);
						// delete nickCommand;
					}
				}
			}
			std::cout << "send msg: " << msg << std::endl;
            // nonBlockingSend(_clients[i], msg, 0);
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
