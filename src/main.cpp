#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include "Server.hpp"
#include "Message.hpp"
#include "ClientChannelMapping.hpp"

bool isAllDigits(const std::string& str) {
    for (std::size_t i = 0; i < str.length(); ++i) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

int main(int argc, char** argv) {
    try {
        if (argc != 3 && argc != 4) {
            throw std::invalid_argument("Usage: " + std::string(argv[0]) + " <port> <password> <optional:ip>");
        }

		int port = atoi(argv[1]);
		if (port < 0 || port > 65535)
			throw std::invalid_argument("Port must be bewteen 0 and 65535");
        if (!isAllDigits(argv[1])) {
            throw std::invalid_argument("Port must be a number.");
        }
		
        if (argc == 3) {
            Server server = Server(argv[2], port, false);
            Msg::_server = &server;
            server.initServer();
        } else if (argc == 4) {
            Server server = Server(argv[2], port, argv[3], false);
            Msg::_server = &server;
            server.initServer();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}

// int main(int argc, char **argv)
// {
//     if (argc != 2)
//     {
//         std::cerr << "Usage: ./" << argv[0] << " <port>" << " <pass>" << std::endl;
//         exit(1);
//     }

//     int port = std::stoi(argv[1]);
//     int sockfd, newsockfd;
//     socklen_t clilen;
//     char buffer[256];
//     std::string bufStr = std::string(buffer);
//     struct sockaddr_in serv_addr, cli_addr;

//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd < 0)
//         error("ERROR opening socket");
//     int enable = 1;
//     if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
//         throw std::runtime_error("Couldn't reuse socket address.");

//     bzero((char *)&serv_addr, sizeof(serv_addr));
//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_addr.s_addr = INADDR_ANY;
//     serv_addr.sin_port = htons(port);

//     if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
//         error("ERROR on binding");

//     listen(sockfd, 5);
//     clilen = sizeof(cli_addr);

//     std::cout << "Server listening on port " << port << "..." << std::endl;

//     while (1)
//     {
//         newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
//         if (newsockfd < 0)
//             error("ERROR on accept");

//         std::cout << "Client connected" << std::endl;

//         char buffer[1024];
//         size_t len = 0;
//         while ((len = read(newsockfd, buffer, sizeof(buffer) - 1)) > 0)
//         {
//             printf("read buffer: %s\n", buffer);
//         }

//         // Send a welcome message to the client
//         std::string welcome_msg = "Welcome to the Simple IRC Server!\n";
//         send_irc_message(newsockfd, welcome_msg);

//         while (1)
//         {
//             bzero(buffer, 256);
//             int n = read(newsockfd, buffer, 255);

//             if (n < 0)
//                 error("ERROR reading from socket");
//             if (n == 0)
//             {
//                 // Connection closed by the client
//                 std::cout << "Client disconnected" << std::endl;
//                 close(newsockfd);
//                 break;
//             }
//                 // Echo the received message back to the client
//             std::cout << "Received: " << buffer;
//             write(newsockfd, buffer, strlen(buffer));
//         }
//     }

//     close(sockfd);
//     return 0;
// }
