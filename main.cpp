#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void send_irc_message(int sockfd, const char *message)
{
    std::string irc_message = "001 :" + std::string(message) + "\r\n";
    write(sockfd, irc_message.c_str(), irc_message.length());
}

void join_channel(int sockfd, const char *channel)
{
    std::string join_message = "JOIN " + std::string(channel) + "\r\n";
    write(sockfd, join_message.c_str(), join_message.length());
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        exit(1);
    }

    int port = std::stoi(argv[1]);
    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[256];
    std::string bufStr = std::string(buffer);
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);



    std::cout << "Server listening on port " << port << "..." << std::endl;

    while (1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");

        std::cout << "Client connected" << std::endl;

        // Send a welcome message to the client
        const char *welcome_msg = "Welcome to the Simple IRC Server!\n";
        send_irc_message(newsockfd, welcome_msg);

        while (1)
        {
            bzero(buffer, 256);
            int n = read(newsockfd, buffer, 255);

            if (n < 0)
                error("ERROR reading from socket");
            if (n == 0)
            {
                // Connection closed by the client
                std::cout << "Client disconnected" << std::endl;
                close(newsockfd);
                break;
            }
                // Echo the received message back to the client
            std::cout << "Received: " << buffer;
            write(newsockfd, buffer, strlen(buffer));
        }
    }

    close(sockfd);
    return 0;
}
