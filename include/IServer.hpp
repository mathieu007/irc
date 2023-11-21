#include <vector>
#include "Client.hpp"
#include "Channel.hpp"
using std::vector;
class IServer {
	Channel *join(Client *client, string channel);
	Channel *join(Client *client, string channel, string key);
	vector<Channel *> getClientChannels(Client *client);
	vector<Channel *> getClientChannels(string username);
	Client *getClient(string username);
	bool isModerator(Client *client);
	bool isInChannel(Client *client, string channel);
	bool isInChannel(string username, string channel);
	bool isInChannel(Client *client, string channel, string key);
	bool channelExist(string channel);
	bool channelExist(string channel, string key);
	bool hasTopic(string &channelName);
	Channel *getChannel(string &channelName);
	Channel *getChannel(string &channelName, string &key);
	Channel *isValidChannelKey(string &channelName, string &key);
	Channel *removeClientFromChannel(Client *client, string &channelName);
	Channel *removeClientFromChannel(Client *client, string &channelName, string &key);
	Channel *removeClientFromChannel(string username, string &channelName);
	Channel *removeClientFromChannel(string username, string &channelName, string &key);
	Channel *banClientFromChannel(Client *client, string &channelName);
	Channel *banClientFromChannel(Client *client, string &channelName, string &key);
	Channel *addClientToChannel(Client *client, string &channelName);
	Channel *addClientToChannel(Client *client, string &channelName, string &key);
	Channel *addTopicToChannel(string &topic, string &channelName);
	Channel *addTopicToChannel(string &topic, string &channelName, string &key);
	Channel *removeTopicFromChannel(string &channelName);
	Channel *removeTopicFromChannel(string &channelName, string &key);
	Channel *removeChannel(string &channelName);
	Channel *removeChannel(string &channelName, string &key);

	bool nickNameExist(string &ncikName);
};