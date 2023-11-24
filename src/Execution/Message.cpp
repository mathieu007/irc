#include "Message.hpp"
#include "CommandFactory.hpp"
#include "String.hpp"

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
ssize_t sendMsg(Client *client, string &data, int flags)
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

ssize_t sendQueuedMsg(Client *client, int flags)
{
	ssize_t bytesSent = 0;
	// if a previous io operation failed or was blocking, we need to get the leftover bytes from the client, and resume sending...
	std::string msg = client->getMsgQueue();
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
			client->setMsgQueue(ptr);
			break;
		}
		else
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				client->setMsgQueue(ptr);
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
	client->getMsgQueue().clear();
	return bytesSent;
}

bool sendAuthMessages(Client *client)
{
	std::string msg;
	if (client->getPass() == "")
	{
		msg = "ERROR :No Password set, configure your irc client, or use /pass <password> if using nc.\r\n";
		sendMsg(client, msg, 0);
	}
	if (client->getUsername() == "")
	{
		msg = "ERROR :No Username set, configure your irc client, or use /user <username> if using nc.\r\n";
		sendMsg(client, msg, 0);
	}
	if (client->getNickname() == "")
	{
		msg = "ERROR :No Username set, configure your irc client, or use /nick <username> if using nc.\r\n";
		sendMsg(client, msg, 0);
	}
	return true;
}

// you absolutely need this if the sender is not the same as the recipient for non blocking io!!!!!
ssize_t sendMsgToRecipient(Client *sender, Client *recipient, string &msg, int flags)
{
	ssize_t byteToSend = 0;

	if (sender == recipient)
		return sendMsg(recipient, msg, flags);
	string msgQueue = recipient->getMsgQueue();
	msgQueue.append(msg);
	byteToSend = msgQueue.length();
	recipient->setMsgQueue(msgQueue);
	return byteToSend;
}

string recvMsg(int sockfd, char *buffer, int flags)
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
			// iff no data available or the call is blocking we don't wait and send the msg right away...
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

bool parseAndExec(Client *client, string &msg, Server &server)
{
	(void)server;
	static CommandFactory cmdFactory;
	// if the string is not ending with \r\n then we do not clear the buffer and wait for the rest of the string.
	if (String::endsWith(msg, "\r\n"))
	{
		cmdFactory.tokenMessage(msg, client, server);
		return true;
	}
	return false;
}
