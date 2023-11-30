#include "Client.hpp"
#include "Channel.hpp"
#include "Vector.hpp"

Client::Client()
{
    this->_isRegistered = false;
    this->_lastRequestTime = time(nullptr);
    this->_nextAllowedConnectionTime = 0;
    this->_numRequests = 0;
    this->_reqSize = 0;
    this->_isBanned = false;
    this->_pass = std::string();
    this->_nickName = std::string();
    this->_userName = std::string();
    this->_msg = "";
    this->_msgSendQueue = std::string();
    this->_msgRecvQueue = std::string();
    this->_address = "";
    this->_port = "";
    this->_remove = false;
    this->_kickedChannels = vector<Channel *>();
    this->_channels = Map<string, Channel *>();
    this->_realName = "";
    this->_socket = 0;
}

Client::~Client() {}

bool Client::operator==(const Client &cmp) const
{
    return this->getUsername() == cmp.getUsername();
}

bool Client::operator!=(const Client &cmp) const
{
    return this->getUsername() != cmp.getUsername();
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

void Client::setIsRegistered()
{
    _isRegistered = true;
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

bool Client::isRegistered() const
{
    return _isRegistered && !_isBanned;
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

Map<string, Channel *> &Client::getChannels()
{
    return _channels;
}

vector<Channel *> &Client::getKickedChannels()
{
    return _kickedChannels;
}

long Client::getLastActivity()
{
    return _lastActivityTime;
}

void Client::setLastActivity(long lastActivityTime)
{
    _lastActivityTime = lastActivityTime;
}

void Client::setSocketIndex(int socketIndex)
{
    _socketIndex = socketIndex;
}

int Client::getSocketIndex() const
{
    return _socketIndex;
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
    time_t currentTime = std::time(nullptr);

    // Convert time_t to long
    long seconds = static_cast<long>(currentTime);

    return seconds;
}

bool Client::canMakeRequest()
{
    _numRequests++;
    long curTime = getCurTime();
    if (_lastRequestTime == 0)
    {
        _lastRequestTime = curTime;
        return true;
    }
    long diff = curTime - _lastRequestTime;
    if (diff == 0)
        diff = 1;
    if (_numRequests / diff >= MAX_REQ_PER_SEC)
        return false;
    _numRequests = 1;
    _lastRequestTime = curTime;
    return true;
}

bool Client::canConnect() const
{
    long time = getCurTime();
    if (time >= this->_nextAllowedConnectionTime || _nextAllowedConnectionTime == 0)
        return true;
    return false;
}

bool Client::isBannned() const
{
    return this->_isBanned;
}

bool Client::isValidUserInfo() const
{
    return (_pass != "" && _nickName != "" && _nickName != "guest" && _userName != "");
}

bool Client::passIsEmpty() const
{
    return _pass == "";
}

bool Client::hasReachMaxReq()
{
    if (this->_numRequests >= MAX_REQ_BEFORE_BAN)
    {
        _isBanned = true;
        return true;
    }
    return false;
}

bool Client::addToChannel(Channel *channel)
{
    if (!channel)
        return false;
    string id = channel->getId();
    // if is in kicked channel then cannot add to channel;
    if (Vector::isIn(_kickedChannels, *channel, &Channel::getId))
        return false;
    if (_channels.hasKey(id))
        return channel;
    if (_channels.addIfNotExist(id, channel))
        return channel;
    return true;
}

bool Client::addToKickedChannel(Channel *channel)
{
    if (!channel)
        return false;
    string id = channel->getId();
    // if is in kicked channel then cannot add to channel;
    if (Vector::isIn(_kickedChannels, *channel, &Channel::getId))
        return false;
    _kickedChannels.push_back(channel);
    return true;
}

bool Client::isInChannel(Channel *channel)
{
    if (!channel)
        return false;
    string id = channel->getId();
    if (_channels.hasKey(id))
        return true;
    return false;
}

bool Client::isKickedFromChannel(Channel *channel)
{
    if (!channel)
        return false;
    string id = channel->getId();
    if (Vector::isIn(_kickedChannels, *channel, &Channel::getId))
        return true;
    return false;
}

bool Client::removeFromChannel(Channel *channel)
{
    if (!channel)
        return false;
    string id = channel->getId();
    if (_channels.remove(id))
        return true;
    return false;
}

bool Client::removeFromKickChannel(Channel *channel)
{
    if (!channel)
        return false;
    string id = channel->getId();
    if (Vector::removeWhere(_kickedChannels, &Channel::getId, id))
        return true;
    return false;
}
