#include <string>


class ClientResult;
class ChannelResult;
class ClientsChannelsResult;

class Db
{
public:
    static ClientsChannelsResult clientsChannels;
    static ClientResult clients;
    static ChannelResult channels;
};