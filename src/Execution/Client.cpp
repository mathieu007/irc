#include "Client.hpp"
#include "Channel.hpp"
#include "ClientChannelMapping.hpp"

Client::Client(Vec<ClientChannelMapping> *mapping)
{
    this->_lastRequestTime = time(nullptr);
    this->_nextAllowedConnectionTime = 0;
    this->_numRequests = 0;
    this->_reqSize = 0;
    this->_isAuth = false;
    this->_pass = std::string();
    this->_nickName = std::string();
    this->_userName = std::string();
    this->_msg = "";
    this->_msgSendQueue = std::string();
    this->_msgRecvQueue = std::string();
    this->_address = "";
    this->_port = "";
    this->_bannedFromServer = false;
    this->_remove = false;
    this->_clientChannelsMapping = mapping;
    this->_realName = "";
    this->_socket = 0;
}

Client::~Client()
{
    // Vec<ClientChannelMapping> map = getMapping();
    // for (size_t i = 0; i < map.size(); i++)
    // {
    //     map[i]->getChannel()->getBannedClients().removeWhere(&Client::getUsername, this->getUsername(), false);
    // }
}

void Client::setIsAuth(bool isAuth)
{
    _isAuth = isAuth;
}

bool Client::operator==(const Client &cmp) const
{
    return this->getUsername() == cmp.getUsername();
}

bool Client::operator!=(const Client &cmp) const
{
    return this->getUsername() != cmp.getUsername();
}

Vec<ClientChannelMapping> Client::getMapping()
{
    Vec<ClientChannelMapping> map = _clientChannelsMapping->where(&ClientChannelMapping::getClientUsername, this->getUsername());
    return map;
}

void Client::setHost(string host)
{
    _host = host;
}

void Client::setSocket(int socket)
{
    _socket = socket;
}

void Client::setNickname(std::string nickName)
{
    _nickName = nickName;
}

void Client::setUsername(std::string userName)
{
    _userName = userName;
}

void Client::setRealname(std::string realName)
{
    _realName = realName;
}

void Client::setRemove(bool remove)
{
    _remove = remove;
}
bool Client::shouldBeRemoved() const
{
    return _remove;
}

string Client::getHost() const
{
    return _host;
}

string Client::getPort() const
{
    return _port;
}

int Client::getSocket() const
{
    return _socket;
}

const string &Client::getNickname() const
{
    return _nickName;
}

const string &Client::getUsername() const
{
    return _userName;
}

const string &Client::getPass() const
{
    return _pass;
}

string &Client::getMsg()
{
    return _msg;
}

string &Client::getMsgSendQueue()
{
    return _msgSendQueue;
}

string &Client::getMsgRecvQueue()
{
    return _msgRecvQueue;
}

Vec<Channel> Client::getInviteList()
{
    Vec<Channel> vec = getMapping().where(&ClientChannelMapping::getIsInvited, true).select(&ClientChannelMapping::getChannel);
    return vec;
}

Vec<Channel> Client::getChannels()
{
    Vec<Channel> vec = getMapping().select(&ClientChannelMapping::getChannel);
    return vec;
}

Vec<Channel> Client::getBannedChannels()
{
    Vec<Channel> vec = getMapping().where(&ClientChannelMapping::getIsBanned, true).select(&ClientChannelMapping::getChannel);
    return vec;
}

long Client::getLastActivity()
{
    return _lastActivityTime;
}

void Client::setLastActivity(long lastActivityTime)
{
    _lastActivityTime = lastActivityTime;
}

void Client::setMsg(string msg)
{
    _msg = msg;
}

void Client::setMsgSendQueue(string msg)
{
    _msgSendQueue = msg;
}

void Client::setMsgRecvQueue(string msg)
{
    _msgRecvQueue = msg;
}

void Client::setPort(string port)
{
    _port = port;
}

// do
void Client::setPass(string pass)
{
    _pass = pass;
}

void Client::setAddress(string address)
{
    _address = address;
}

void Client::incrementRequest()
{
    _numRequests++;
}
void Client::incrementReqSize(long reqSize)
{
    _reqSize += reqSize;
}

/// version cpp11
// long Client::getCurTime() const
// {
//     std::chrono::_V2::system_clock::duration durationSinceEpoch = std::chrono::system_clock::now().time_since_epoch();
//     long seconds = std::chrono::duration_cast<std::chrono::seconds>(durationSinceEpoch).count();
//     return seconds;
// }

////version cpp98
long Client::getCurTime() const
{
    // Get the current time in seconds since the epoch
    time_t currentTime = std::time(NULL);

    // Convert time_t to long
    long seconds = static_cast<long>(currentTime);

    return seconds;
}

bool Client::getBannedFromServer()
{
    return _bannedFromServer;
}

void Client::setBannedFromServer(bool banned)
{
    _bannedFromServer = banned;
}

bool Client::canMakeRequest()
{
    _numRequests++;
    long curTime = getCurTime();
    long prevRequest = _lastRequestTime;
    _lastRequestTime = curTime;
    if (prevRequest == 0)
        return true;
    long diff = curTime - prevRequest;
    if (diff == 0)
    {
        if (_numRequests >= MAX_REQ_PER_SEC)
            return false;
        return true;
    }
    if (_numRequests / diff >= MAX_REQ_PER_SEC)
        return false;
    _numRequests = 1;
    return true;
}

void Client::setNextAllowedConnectionTime(long time)
{
    long cur = getCurTime();
    _nextAllowedConnectionTime = cur + time;
}

bool Client::isValidUserInfo() const
{
    return (_pass != "" && _nickName != "" && _nickName != "guest" && _userName != "" && _isAuth);
}

bool Client::passIsEmpty() const
{
    return _pass == "";
}

bool Client::hasReachMaxReq()
{
    if (this->_numRequests >= MAX_REQ_BEFORE_BAN)
        return true;
    return false;
}

bool Client::canJoinChannel(Channel *channel)
{
    if (!channel)
        return false;
    if (channel->getNumClients() >= channel->getMaxNumClients())
        return false;
    if (channel->isBanned(this))
        return false;
    if (channel->isOnInvitationOnly() && channel->isInIvitationList(this))
        return true;
    if (!channel->isOnInvitationOnly())
        return true;
    return false;
}

bool Client::addChannel(Channel *channel)
{
    if (!channel)
        return false;
    ClientChannelMapping *map = getMapping().first(&ClientChannelMapping::getChannelName, channel->getName());
    if (map != nullptr)
        return false;
    map = new ClientChannelMapping(this, channel);
    _clientChannelsMapping->push_back(map);
    return true;
}

bool Client::addToKickedChannel(Channel *channel)
{
    if (!channel || channel->getSuperModerator() == this)
        return false;
    return channel->addBannedClient(this);
}

bool Client::isInChannel(Channel *channel)
{
    if (!channel)
        return false;
    ClientChannelMapping *map = getMapping().first(&ClientChannelMapping::getChannelName, channel->getName());
    if (map && !map->getIsBanned())
        return true;
    // if (channel->isOnInvitationOnly() && channel->isInIvitationList(this))
    //     return true;
    return false;
}

bool Client::isKickedFromChannel(Channel *channel)
{
    if (!channel)
        return false;
    if (channel->getBannedClients().exist(&Client::getUsername, this->getUsername()))
        return true;
    return false;
}

bool Client::removeFromChannel(Channel *channel)
{
    if (!channel)
        return false;
    ClientChannelMapping *map = getMapping().first(&ClientChannelMapping::getChannelName, channel->getName());
    if (map)
    {
        _clientChannelsMapping->remove(map, true);
        channel->setNumClients(channel->getNumClients() - 1);
        return true;
    }
    return false;
}

bool Client::removeFromKickChannel(Channel *channel)
{
    if (!channel)
        return false;
    if (channel->getBannedClients().exist(&Client::getUsername, this->getUsername()))
        channel->getBannedClients().removeWhere(&Client::getUsername, this->getUsername(), false);
    return false;
}
