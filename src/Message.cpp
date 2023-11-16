#include "Message.hpp"

ssize_t nonBlockingSend(Client *client, string &data, int flags)
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

// std::vector<std::string> tokenMessage(std::string message){

// }