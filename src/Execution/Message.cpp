#include "Message.hpp"
#include "CommandFactory.hpp"
#include "String.hpp"
#include "Server.hpp"
#include "Channel.hpp"

Server *Msg::_server = nullptr;
/*
When a socket is in non-blocking mode using:
int flags = fcntl(sockfd, F_GETFL, 0);
fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

select or poll:
Even though you checked the readiness with select,
it's possible that the non-blocking operation may still return EWOULDBLOCK or EAGAIN.
In such cases, you should handle these errors appropriately and try the operation again later.

send:
If the socket's send buffer is full, send may return immediately
with an error (EAGAIN or EWOULDBLOCK) instead of waiting for space in the buffer.

recv:
If there is no data available in the socket's receive buffer,
recv may return immediately with an error (EAGAIN or EWOULDBLOCK) instead of waiting for data to arrive.
*/

// we need to put the leftover data in a queue so it's processed later.
ssize_t Msg::sendMsg(Client *client, string &data, int flags)
{
	ssize_t bytesSent = 0;
	// if a previous io operation failed or was blocking, we need to get the leftover bytes from the client, and resume sending...
	std::string msg = client->getMsg();
	msg.append(data);
	int len = msg.length();
	const char *ptr = msg.c_str();
	while (bytesSent < (ssize_t)len)
	{
		ptr = ptr + bytesSent;
		ssize_t result = send(client->getSocket(), ptr, len - bytesSent, flags);
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
				std::cerr << "send EAGAIN || EWOULDBLOCK error, continue processing..." << std::endl;
				return bytesSent;
			}
			else if (errno == EINTR)
			{
				// Interrupted by a signal, continue sending ????????????
				client->setRemove(true);
				perror("send interupted by signal");
				break;
			}
			else
			{
				// Error occurred
				client->setRemove(true);
				perror("send interupted by peer");
				break;
			}
		}
	}
	client->getMsg().clear();
	return bytesSent;
}

ssize_t Msg::sendQueuedMsg(Client *client, int flags)
{
	ssize_t bytesSent = 0;
	// if a previous io operation failed or was blocking, we need to get the leftover bytes from the client, and resume sending...
	std::string msg = client->getMsgSendQueue();
	int len = msg.length();
	const char *ptr = msg.c_str();
	while (bytesSent < (ssize_t)len)
	{
		ptr = ptr + bytesSent;
		ssize_t result = send(client->getSocket(), ptr, len - bytesSent, flags);
		// the data is not sent in one swoop we must handle the data per client, we need to save the left over msg data into user msg
		if (result > 0)
			bytesSent += result;
		else if (result == 0 && len == bytesSent)
			break;
		else
		{
			/// in some situation send was returning an error, but not a fatal error
			/// while EAGAIN or EWOULDBLOCK we should not cancel the client processing...
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				client->setMsgSendQueue(ptr);
				std::cerr << "send EAGAIN || EWOULDBLOCK error, continue processing..." << std::endl;
				return bytesSent;
			}
			else if (errno == EINTR)
			{
				// Interrupted by a signal, continue sending ????????????
				client->setRemove(true);
				perror("send interupted by signal");
				break;
			}
			else
			{
				client->setRemove(true);
				perror("send interupted by peer");
				break;
			}
		}
	}
	client->setMsgSendQueue("");
	return bytesSent;
}

bool Msg::sendAuthMessages(Client *client)
{
	std::string msg;
	if (client->getPass() == "")
	{
		msg = "ERROR : Cannot execute command, No Password set, configure your irc client, or use PASS <password> if using nc.\r\n";
		Msg::sendMsg(client, msg, 0);
	}
	else if (client->getUsername() == "")
	{
		msg = "ERROR : Cannot execute command, No Username set, configure your irc client, or use USER <username> * 0 <realname> if using nc.\r\n";
		Msg::sendMsg(client, msg, 0);
	}
	else if (client->getNickname() == "" || client->getNickname() == "guest")
	{
		msg = "ERROR : Cannot execute command, No Nick name set, configure your irc client, or use NICK <username> if using nc.\r\n";
		Msg::sendMsg(client, msg, 0);
	}
	return true;
}

// you absolutely need this if the sender is not the same as the recipient for non blocking io!!!!!
ssize_t Msg::sendMsgToRecipient(Client *sender, Client *recipient, string &msg, int flags)
{
	ssize_t byteToSend = 0;

	if (*sender == *recipient)
		return Msg::sendMsg(recipient, msg, flags);
	string msgQueue = recipient->getMsgSendQueue();
	msgQueue.append(msg);
	byteToSend = msgQueue.length();
	recipient->setMsgSendQueue(msgQueue);
	return byteToSend;
}

string Msg::recvMsg(int sockfd, char *buffer)
{
	std::string msg = std::string();
	ssize_t bytesRead = 0;
	while (true)
	{
		bytesRead = recv(sockfd, buffer, MAX_BUFFER_SIZE, 0);
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
			// iff no data available or the call is blocking we don't wait and send the msg right away...
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				cout << "recv EAGAIN || EWOULDBLOCK error, continue processing..." << std::endl;
				return msg;
			}
			else if (errno == EINTR)
			{
				// should we continue ??????????
				vector<Client *> clients = Msg::_server->getClients();
				Client *client = clients[sockfd];
				if (client)
					client->setRemove(true);
				break;
			}
			else
			{
				// Error occurred
				vector<Client *> clients = Msg::_server->getClients();
				Client *client = clients[sockfd];
				if (client)
					client->setRemove(true);
				perror("recv interupted by peer");
				break;
			}
		}
	}
	return msg;
}

bool Msg::parseAndExec(Client *client, string &msg, Server &server)
{
	(void)server;
	string extracted;
	static CommandFactory cmdFactory;
	// if the string is not ending with \r\n then we do not clear the buffer and wait for the rest of the string.
	while (!msg.empty())
	{
		extracted = String::extractUptoFirstOccurence(msg, "\r\n", false);
		if (extracted.empty())
			return true;
		cmdFactory.tokenMessage(extracted, client, server);
		if (msg.length() - extracted.length() == 0)
			msg = string("");
		else
			msg.erase(msg.begin(), msg.begin() + extracted.length());
		client->setMsgRecvQueue(msg);
	}
	return true;
}

void Msg::sendUserlist(Client *client, string &channelName, Server &server)
{
	std::string userList;
	std::string messageToClientList;
	Channel *channel = server.getChannel(channelName);
	std::vector<Client *> clients = server.getClientsInAChannel(channel);
	for (std::vector<Client *>::size_type i = 0; i < clients.size(); ++i)
	{
		if (channel && channel->isModerator(clients[i]))
			userList += "@" + clients[i]->getNickname() + " ";
		else
			userList += clients[i]->getNickname() + " ";
	}
	for (std::vector<Client *>::size_type i = 0; i < clients.size(); ++i)
	{
		messageToClientList = "353 " + client->getNickname() + " = " + channelName + " :" + userList + "\r\n";
		std::cout << YELLOW << "message sent to client:" << messageToClientList << RESET << std::endl;
		Msg::sendMsgToRecipient(client, clients[i], messageToClientList, 0);
		messageToClientList = "366 " + client->getNickname() + " " + channelName + " :End of /NAMES list\r\n";
		Msg::sendMsgToRecipient(client, clients[i], messageToClientList, 0);
	}
}
