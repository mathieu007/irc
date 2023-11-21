#pragma once
#include <vector>
#include "Client.hpp"
#include "IChannel.hpp"
using std::vector;

class IServer
{

public:
	virtual ~IServer();
	IChannel *join(Client *client, string &channel);
	IChannel *join(Client *client, string &channel, string &key);
	vector<IChannel *> getClientChannels(Client *client);
	vector<IChannel *> getClientChannels(string &username);
	Client *getClient(string &username);
	bool isModerator(Client *client);
	bool isInChannel(Client *client, string &channel);
	bool isInChannel(string &username, string &channel);
	bool isInChannel(Client *client, string &channel, string &key);
	bool channelExist(string &channel);
	bool channelExist(string &channel, string key);
	bool hasTopic(string &channelName);
	IChannel *getChannel(string &channelName);
	IChannel *getChannel(string &channelName, string &key);
	IChannel *isValidChannelKey(string &channelName, string &key);
	IChannel *removeClientFromChannel(Client *client, string &channelName);
	IChannel *removeClientFromChannel(Client *client, string &channelName, string &key);
	IChannel *removeClientFromChannel(string &username, string &channelName);
	IChannel *removeClientFromChannel(string &username, string &channelName, string &key);
	IChannel *banClientFromChannel(Client *client, string &channelName);
	IChannel *banClientFromChannel(Client *client, string &channelName, string &key);
	IChannel *addClientToChannel(Client *client, string &channelName);
	IChannel *addClientToChannel(Client *client, string &channelName, string &key);
	IChannel *addTopicToChannel(string &topic, string &channelName);
	IChannel *addTopicToChannel(string &topic, string &channelName, string &key);
	IChannel *removeTopicFromChannel(string &channelName);
	IChannel *removeTopicFromChannel(string &channelName, string &key);
	IChannel *removeChannel(string &channelName);
	IChannel *removeChannel(string &channelName, string &key);

	bool nickNameExist(string &ncikName);
};