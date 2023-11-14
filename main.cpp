#include <netdb.h>
#include <cstring>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <arpa/inet.h>


using namespace std;

int get_port(char **argv)
{
    int port = std::stoi(argv[1]);
    if (port < 1 || port > 65535)
        throw std::runtime_error("Invalid port number.");
    return port;
}

void handleClientMessage(const char *message)
{
    cout << "Received message: " << message << endl;
    // Implement your message handling logic here
}

int create_socket(void)
{
    int listenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenFd < 0)
        throw std::runtime_error("Couldn't create socket.");
    int enable = 1;
    if (setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        throw std::runtime_error("Couldn't reuse socket address.");
    return listenFd;
}

void bind_listen(int listenFd, int portNo)
{
    struct sockaddr_in svrAddress;
    memset(&svrAddress, 0, sizeof(svrAddress));
    svrAddress.sin_family = AF_INET;
    svrAddress.sin_addr.s_addr = inet_addr("172.31.200.244");
    svrAddress.sin_port = htons(portNo);

    if (bind(listenFd, (struct sockaddr *)&svrAddress, sizeof(svrAddress)) < 0)
        throw std::runtime_error("Could not bind the socket FD the local address.");
    if (listen(listenFd, 5) == -1)
        throw std::runtime_error("Could not accept connections on socket FD");
}

int main(int argc, char **argv)
{
    if (argc != 3)
        throw std::runtime_error("Error: invalid number of arguments should be: ./ircserv <port> <password>\n");

    int port = get_port(argv);
    int listenFd = create_socket();
    bind_listen(listenFd, port);

    // Server *server = NULL;
    // string servername = "IRC_SERV";
    // encode password sha256 in server

    // creating the loop
    while (true)
    {
        struct sockaddr_in clientAddress;
        socklen_t clientLength = sizeof(clientAddress);
        int clientSocket = accept(listenFd, (struct sockaddr *)&clientAddress, &clientLength);

        if (clientSocket < 0)
        {
            throw std::runtime_error("Error accepting connection.");
        }

        char buffer[1024];
        ssize_t bytesRead = read(clientSocket, buffer, sizeof(buffer));

        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0'; // Null-terminate the received message
            handleClientMessage(buffer);

            const char *response = "Welcome to the IRC server!";
            write(clientSocket, response, strlen(response));

        }
        // close(clientSocket);
    }
}