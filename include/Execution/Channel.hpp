#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "Client.hpp"

using std::string;
using std::vector;
class Channel
{
private:
    string _id;
    string _name;
    string _key;
    string _topic;
    bool _hasTopic;
    Client *_moderator;

public:
    Channel(string &name);
    Channel(string &name, string &key);
    ~Channel();
    bool hasTopic() const;
    const string &getName() const;
    const string &getKey() const;
    const string &getId() const;
    Client *getModerator();
    void setModerator(Client *moderator);
    void setTopic(string &topic);
};