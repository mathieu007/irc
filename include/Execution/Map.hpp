#pragma once
#include <iostream>
#include <string>
#include <iterator>
#include <map>
#include "IMap.hpp"

template <typename TKey, typename T>
class Map : public std::map<TKey, T>, public IMap<TKey, T>
{
public:
    Map() {}
    ~Map()
    {
        removeAll();
    }
    bool hasKey(TKey &key)
    {
        if (this->empty())
            return false;
        if (this->find(key) == this->end())
            return false;
        return true;
    }

    bool empty()
    {
        return this->empty();
    }

    bool tryGet(TKey &key, T &val)
    {
        if (this->empty())
            return false;
        typename std::map<TKey, T>::const_iterator it = this->find(key);
        if (it == this->end())
            return false;
        val = (*it).second;
        return true;
    }

    T &get(TKey &key)
    {
        typename std::map<TKey, T>::iterator it = this->find(key);
        return (*it).second;
    }

    bool remove(TKey &key)
    {
        if (this->empty())
            return false;
        typename std::map<TKey, T>::iterator it = this->find(key);
        if (it == this->end())
            return false;
        this->erase(it);
        return true;
    }

    bool removeAll()
    {
        if (this->empty())
            return false;
        typename std::map<TKey, T>::iterator it = this->begin();
        typename std::map<TKey, T>::iterator end = this->end();
        while (it != end)
        {
            delete &(*it).second;
        }
        return true;
    }

    void addOrReplace(TKey &key, T &val)
    {
        (*this)[key] = val;
    }

    bool replace(TKey &key, T &val)
    {
        if (!this->hasKey(key))
            return false;
        (*this)[key] = val;
        return true;
    }

    bool addIfNotExist(TKey &key, T &val)
    {
        if (!this->hasKey(key))
        {
            (*this)[key] = val;
            return true;
        }
        (*this)[key] = val;
        return false;
    }
};