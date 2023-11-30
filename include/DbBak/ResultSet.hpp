#pragma once
#include <string>
#include <vector>

using std::vector;

template <typename T>
class ResultSet
{
protected:
    vector<T *> _data;

public:
    ResultSet()
    {
        _data = vector<T *>();
    };
    T *first()
    {
        if (_data.size() == 0)
            return nullptr;
        return _data[0];
    };
    T *one()
    {
        if (_data.size() != 1)
            return nullptr;
        return _data[0];
    };
    T *last()
    {
        if (_data.size() == 0)
            return nullptr;
        return _data[_data.size() - 1];
    };
    T *at(int index)
    {
        if (index >= _data.size())
            return nullptr;
        return _data[index];
    };
    bool hasResult() const
    {
        if (_data.size() == 0)
            return false;
        return true;
    };
    vector<T *> results()
    {
        return _data;
    };
};
