#pragma once
#include <iostream>
#include <string>
#include <iterator>
#include <vector>

using std::vector;

class Vector
{
public:
    template <typename T, typename TReturn>
    static vector<TReturn *> select(vector<T *> &values, TReturn *(T::*getter)() const)
    {
        if (values.empty())
            return vector<TReturn *>();
        typedef typename vector<T *>::const_iterator Iterator;
        vector<TReturn *> vec = vector<TReturn *>();
        for (Iterator it = values.begin(); it != values.end(); it++)
        {
            if (*it)
                vec.push_back(((*(*it)).*getter)());
        }
        return vec;
    }

    // find by address
    template <typename T>
    static bool remove(vector<T *> &values, T *valueAddr)
    {
        if (values.empty() || valueAddr == nullptr)
            return false;
        typedef typename vector<T *>::iterator Iterator;
        for (Iterator it = values.begin(); it != values.end(); it++)
        {
            if (*it == valueAddr)
            {
                values.erase(it);
                return true;
            }
        }
        return false;
    }

    // find by address
    template <typename T>
    static bool exist(vector<T *> &values, T *valueAddr)
    {
        if (values.empty())
            return false;
        typedef typename vector<T *>::const_iterator Iterator;
        for (Iterator it = values.begin(); it != values.end(); it++)
        {
            if (*it == valueAddr)
                return true;
        }
        return false;
    }

    template <typename T, typename TPropValue>
    static bool exist(vector<T *> &values, const TPropValue &(T::*getter)() const, const TPropValue &value)
    {
        if (values.empty())
            return false;
        return Vector::where(values, getter, value).size() > 0;
    }

    template <typename T, typename TPropValue>
    static bool exist(vector<T *> &values, TPropValue (T::*getter)() const, TPropValue value)
    {
        if (values.empty())
            return false;
        return Vector::where(values, getter, value).size() > 0;
    }

    template <typename T, typename TPropValue>
    static vector<T *> where(const vector<T *> &values, T &instance, TPropValue (T::*getter)() const)
    {
        if (values.empty())
            return vector<T *>();
        typedef typename vector<T *>::const_iterator Iterator;
        vector<T *> vec = vector<T *>();
        for (Iterator it = values.begin(); it != values.end(); it++)
        {
            if (*it && (instance.*getter)() == ((*(*it)).*getter)())
                vec.push_back(*it);
        }
        return vec;
    }

    template <typename T, typename TPropValue>
    static vector<T *> where(const vector<T *> &values, T &instance, const TPropValue &(T::*getter)() const)
    {
        if (values.empty())
            return vector<T *>();
        typedef typename vector<T *>::const_iterator Iterator;
        vector<T *> vec = vector<T *>();
        for (Iterator it = values.begin(); it != values.end(); it++)
        {
            if (*it && (instance.*getter)() == ((*(*it)).*getter)())
                vec.push_back(*it);
        }
        return vec;
    }

    template <typename T, typename TPropValue>
    static vector<T *> where(vector<T *> &values, const TPropValue &(T::*getter)() const, TPropValue &value)
    {
        if (values.empty())
            return vector<T *>();
        typedef typename vector<T *>::const_iterator Iterator;
        vector<T *> vec = vector<T *>();
        for (Iterator it = values.begin(); it != values.end(); it++)
        {
            if (*it && value == ((*(*it)).*getter)())
                vec.push_back(*it);
        }
        return vec;
    }

    template <typename T, typename TPropValue>
    static vector<T *> where(vector<T *> &values, const TPropValue &(T::*getter)() const, const TPropValue &value)
    {
        if (values.empty())
            return vector<T *>();
        typedef typename vector<T *>::const_iterator Iterator;
        vector<T *> vec = vector<T *>();
        for (Iterator it = values.begin(); it != values.end(); it++)
        {
            if (*it && value == ((*(*it)).*getter)())
                vec.push_back(*it);
        }
        return vec;
    }

    template <typename T, typename TPropValue>
    static vector<T *> where(vector<T *> &values, TPropValue (T::*getter)() const, TPropValue value)
    {
        if (values.empty())
            return vector<T *>();
        typedef typename vector<T *>::const_iterator Iterator;
        vector<T *> vec = vector<T *>();
        for (Iterator it = values.begin(); it != values.end(); it++)
        {
            if (*it && value == ((*(*it)).*getter)())
                vec.push_back(*it);
        }
        return vec;
    }

    template <typename T, typename TPropValue>
    static bool isIn(const vector<T *> &values, TPropValue *instance, TPropValue *(T::*getter)() const)
    {
        if (values.empty())
            return false;
        typedef typename vector<T *>::const_iterator Iterator;
        for (Iterator it = values.begin(); it != values.end(); it++)
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
        for (Iterator it = values.begin(); it != values.end(); it++)
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
        for (Iterator it = values.begin(); it != values.end(); it++)
        {
            if (*it && value == ((*(*it)).*getter)())
                return true;
        }
        return false;
    }

    template <typename T, typename TPropValue>
    static T *getOne(vector<T *> &values, const TPropValue &(T::*getter)() const, const TPropValue &value)
    {
        if (values.empty())
            return nullptr;
        typedef typename vector<T *>::const_iterator Iterator;
        for (Iterator it = values.begin(); it != values.end(); it++)
        {
            if (*it && value == ((*(*it)).*getter)())
                return *it;
        }
        return nullptr;
    }

    template <typename T, typename TPropValue>
    static bool isIn(vector<T *> &values, const TPropValue &(T::*getter)() const, const TPropValue &value)
    {
        if (values.empty())
            return false;
        typedef typename vector<T *>::const_iterator Iterator;
        for (Iterator it = values.begin(); it != values.end(); it++)
        {
            if (*it && value == ((*(*it)).*getter)())
                return true;
        }
        return false;
    }

    template <typename T, typename TPropValue>
    static bool removeWhere(vector<T *> &values, TPropValue (T::*getter)() const, TPropValue value)
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
                it++;
        }
        return successDelete;
    }

    template <typename T, typename TPropValue>
    static bool removeWhere(vector<T *> &values, const TPropValue &(T::*getter)() const, const TPropValue &value)
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
                it++;
        }
        return successDelete;
    }
};