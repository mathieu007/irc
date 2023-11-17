#include "Client.hpp"

Client::Client()
{
    this->_lastRequestTime = time(nullptr);
    this->_nextAllowedConnectionTime = 0;
    this->_numRequests = 0;
    this->_reqSize = 0;
    this->_banned = false;
    this->_pass = "";
    this->_nickName = "";
    this->_username = "";
    this->_msg = "";
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

////////////////// GETTER ///////////////

string Client::getHost() const
{
    return _host;
}

int Client::getSocket() const
{
    return _socket;
}

string Client::getNickname() const
{
    return _nickName;
}

string Client::getUsername() const
{
    return _username;
}

string &Client::getMsg()
{
    return _msg;
}

void Client::setMsg(string msg)
{
    _msg = msg;
}

void Client::setPort(string port)
{
    _port = port;
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

long Client::getCurTime() const
{
    std::chrono::_V2::system_clock::duration durationSinceEpoch = std::chrono::system_clock::now().time_since_epoch();
    long seconds = std::chrono::duration_cast<std::chrono::seconds>(durationSinceEpoch).count();
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
    return this->_banned;
}

bool Client::isGoingToGetBanned()
{
    if (this->_numRequests >= MAX_REQ_BEFORE_BAN)
    {
        _banned = true;
        return true;
    }
    return false;
}

IChannel *Client::addToChanel(string &channelName)
{
    IChannel *channel = this->findByName(channelName);
    if (channel == nullptr)
        return nullptr;
    channel->addClient(this);
    return channel;
}

IChannel *Client::findByName(string &channelName) const
{
    vector<IChannel *>::const_iterator begin = this->_registeredChannels.begin();
    vector<IChannel *>::const_iterator end = this->_registeredChannels.end();
    if (channelName.empty())
        return nullptr;
    while (begin != end)
    {
        if (channelName == (*begin)->getChannelName())
            return *begin;
        begin++;
    }
    return nullptr;
}

IChannel *Client::isInChanel(string &channelName) const
{
    IChannel *found = this->findByName(channelName);
    if (found == nullptr)
        return nullptr;
    return found;
}
