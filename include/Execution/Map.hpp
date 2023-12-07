#pragma once
#include <iostream>
#include <string>
#include <iterator>
#include <map>

template <typename TKey, typename T>
class Map : public std::map<TKey, T>
{
public:
    Map() {}
    // virtual ~Map()
    // {
    //     removeAll();
    // }
    bool hasKey(const TKey &key)
    {
        if (this->isEmpty())
            return false;
        if (this->find(key) == this->end())
            return false;
        return true;
    }

    bool isEmpty()
    {
        return this->empty();
    }

    bool tryGet(const TKey &key, T &val)
    {
        if (this->isEmpty())
            return false;
        typename std::map<TKey, T>::const_iterator it = this->find(key);
        if (it == this->end())
            return false;
        val = (*it).second;
        return true;
    }

    T get(const TKey &key)
    {
        typename std::map<TKey, T>::iterator it = this->find(key);
        return (*it).second;
    }

    bool remove(const TKey &key)
    {
        if (this->isEmpty())
            return false;
        typename std::map<TKey, T>::iterator it = this->find(key);
        if (it == this->end())
            return false;
        this->erase(it);
        return true;
    }

    bool removeAll()
    {
        if (this->isEmpty())
            return false;
        typename std::map<TKey, T>::iterator it = this->begin();
        typename std::map<TKey, T>::iterator end = this->end();
        // while (it != end)
        // {
        //     // delete (*it).second;
        // }
        return true;
    }

    void add(const TKey &key, T val)
    {
        (*this)[key] = val;
    }

    void addOrReplace(const TKey &key, T val)
    {
        (*this)[key] = val;
    }

    bool replace(const TKey &key, T val)
    {
        if (!this->hasKey(key))
            return false;
        (*this)[key] = val;
        return true;
    }

    bool addIfNotExist(const TKey &key, T val)
    {
        if (!this->hasKey(key))
        {
            (*this)[key] = val;
            return true;
        }
        return false;
    }
};