#pragma once
#include <iostream>
#include <iterator>
#include <string>
#include <map>

template <typename TKey, typename T>
class IMap
{
public:
    ~IMap();
    bool empty();
    bool hasKey(TKey &key);
    bool tryGet(TKey &key, T &val);
    T &get(TKey &key);
    bool remove(TKey &key);
    bool removeAll();
    void addOrReplace(TKey &key, T &val);
    bool replace(TKey key, T &val);
    bool addIfNotExist(TKey key, T &val);
};