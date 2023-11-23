#pragma once
#include <iostream>
#include <string>
#include <iterator>
#include <vector>

using std::vector;

class Vector
{
public:
    template <typename T, typename TPropValue>
    static bool isIn(const vector<T *> &values, T &instance, TPropValue (T::*getter)() const)
    {
        if (values.empty())
            return false;
        typedef typename vector<T *>::const_iterator Iterator;
        for (Iterator it = values.begin(); it != values.end(); ++it)
        {
            if (*it && (instance.*getter)() == ((*(*it)).*getter)())
                return true;
        }
        return false;
    }

    template <typename T, typename TPropValue>
    static bool isIn(const vector<T *> &values, T &instance, const TPropValue &(T::*getter)() const)
    {
        if (values.empty())
            return false;
        typedef typename vector<T *>::const_iterator Iterator;
        for (Iterator it = values.begin(); it != values.end(); ++it)
        {
            if (*it && (instance.*getter)() == ((*(*it)).*getter)())
                return true;
        }
        return false;
    }

    template <typename T, typename TPropValue>
    static bool isIn(vector<T *> &values, const TPropValue &(T::*getter)() const, TPropValue &value)
    {
        if (values.empty())
            return false;
        typedef typename vector<T *>::const_iterator Iterator;
        for (Iterator it = values.begin(); it != values.end(); ++it)
        {
            if (*it && value == ((*(*it)).*getter)())
                return true;
        }
        return false;
    }

    template <typename T, typename TPropValue>
    static bool removeWhere(vector<T *> &values, const TPropValue &(T::*getter)() const, TPropValue &value)
    {
        if (values.empty())
            return false;
        bool successDelete = false;
        typedef typename vector<T *>::iterator Iterator;
        Iterator it = values.begin();
        while (it != values.end())
        {
            if (*it && value == ((*(*it)).*getter)())
            {
                successDelete = true;
                it = values.erase(it);
            }
            else
                ++it;
        }
        return successDelete;
    }
};