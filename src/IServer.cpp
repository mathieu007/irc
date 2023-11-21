////NOT A REAL CLASS

#include "IServer.hpp"

bool isModerator(Client *client)
{
	// return true;
	return false;
}
bool isInChannel(Client *client, string channel)
{
	// return true;
	return false;
}
bool isInChannel(string username, string channel)
{
	// return true;
	return false;
}
bool isInChannel(Client *client, string channel, string key)
{
	// return true;
	return false;
}
bool channelExist(string channel)
{
	// return true;
	return false;
}
bool channelExist(string channel, string key)
{
	// return true;
	return false;
}
bool hasTopic(string &channelName)
{
	// return true;
	return false;
}
Channel *getChannel(const std::string &channelName)
{
	// Implement actual logic here
	// return new Channel(channelName);
	return nullptr;
}

Channel *getChannel(const std::string &channelName, const std::string &key)
{
	// Implement actual logic here
	// return new Channel(channelName);
	return nullptr;
}

Channel *isValidChannelKey(const std::string &channelName, const std::string &key)
{
	// Implement actual logic here
	// return new Channel(channelName);
	return nullptr;
}

Channel *removeClientFromChannel(Client *client, const std::string &channelName)
{
	// Implement actual logic here
	// return new Channel(channelName);
	return nullptr;
}

Channel *removeClientFromChannel(Client *client, const std::string &channelName, const std::string &key)
{
	// Implement actual logic here
	// return new Channel(channelName);
	return nullptr;
}

Channel *removeClientFromChannel(const std::string &username, const std::string &channelName)
{
	// Implement actual logic here
	// return new Channel(channelName);
	return nullptr;
}

Channel *removeClientFromChannel(const std::string &username, const std::string &channelName, const std::string &key)
{
	// Implement actual logic here
	// return new Channel(channelName);
	return nullptr;
}

Channel *banClientFromChannel(Client *client, const std::string &channelName)
{
	// Implement actual logic here
	// return new Channel(channelName);
	return nullptr;
}

Channel *banClientFromChannel(Client *client, const std::string &channelName, const std::string &key)
{
	// Implement actual logic here
	// return new Channel(channelName);
	return nullptr;
}

Channel *addClientToChannel(Client *client, const std::string &channelName)
{
	// Implement actual logic here
	// return new Channel(channelName);
	return nullptr;
}

Channel *addClientToChannel(Client *client, const std::string &channelName, const std::string &key)
{
	// Implement actual logic here
	// return new Channel(channelName);
	return nullptr;
}

Channel *addTopicToChannel(const std::string &topic, const std::string &channelName)
{
	// Implement actual logic here
	// return new Channel(channelName);
	return nullptr;
}

Channel *addTopicToChannel(const std::string &topic, const std::string &channelName, const std::string &key)
{
	// Implement actual logic here
	// return new Channel(channelName);
	return nullptr;
}

Channel *removeTopicFromChannel(const std::string &channelName)
{
	// Implement actual logic here
	// return new Channel(channelName);
	return nullptr;
}

Channel *removeTopicFromChannel(const std::string &channelName, const std::string &key)
{
	// Implement actual logic here
	// return new Channel(channelName);
	return nullptr;
}

Channel *removeChannel(const std::string &channelName)
{
	// Implement actual logic here
	// return new Channel(channelName);
	return nullptr;
}

Channel *removeChannel(const std::string &channelName, const std::string &key)
{
	// Implement actual logic here
	// return new Channel(channelName);
	return nullptr;
}

Channel *join(Client *client, const std::string &channel)
{
	// Implement actual logic here
	// return new Channel(channel);
	return nullptr;
}

Channel *join(Client *client, const std::string &channel, const std::string &key)
{
	// Implement actual logic here
	// return new Channel(channel);
	return nullptr;
}

std::vector<Channel *> getClientChannels(Client *client)
{
	// Implement actual logic here
	// return std::vector<Channel *>();
	return std::vector<Channel *>(); // return an empty vector
}

std::vector<Channel *> getClientChannels(const std::string &username)
{
	// Implement actual logic here
	return std::vector<Channel *>(); // return an empty vector
}
Client *getClient(const std::string &username)
{
	// Implement actual logic here
	// return nullptr;
	return nullptr; // replace nullptr with actual logic
}

bool IServer::nickNameExist(std::string &ncikName){
	return false;
}