#pragma once
#include <iostream>
#include <vector>

template <typename T>
class Vec : public std::vector<T *>
{
public:
    typedef typename std::vector<T *>::iterator Iterator;
    Vec() : std::vector<T *>() {}
    Vec(int count) : std::vector<T *>(count) {}

    template <typename TReturn>
    Vec<TReturn> select(TReturn *(T::*getter)())
    {
        if (this->empty())
            return Vec<TReturn>();
        Vec<TReturn> vec = Vec<TReturn>();
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it)
                vec.push_back(((*it)->*getter)());
        }
        return vec;
    };

    template <typename TReturn>
    Vec<TReturn> select(TReturn &(T::*getter)())
    {
        if (this->empty())
            return Vec<TReturn>();
        Vec<TReturn> vec = Vec<TReturn>();
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it)
                vec.push_back(((*it)->*getter)());
        }
        return vec;
    };

    bool remove(T *valueAddr, bool deletePtr)
    {
        if (valueAddr == nullptr || this->empty())
            return false;
        Iterator it = this->begin();
        while (it != this->end())
        {
            if (*it == valueAddr)
            {
                if (deletePtr)
                {
                    delete *it;
                    *it = nullptr;
                }
                it = this->erase(it);
                return true;
            }
            it++;
        }
        return false;
    };

    bool removeNulls()
    {
        if (this->empty())
            return false;
        Iterator it = this->begin();
        while (it != this->end())
        {
            if (*it == nullptr)
                it = this->erase(it);
            else
                it++;
        }
        return false;
    };

    bool remove(T &valueAddr)
    {
        if (this->empty())
            return false;
        Iterator it = this->begin();
        while (it != this->end())
        {
            if ((*(*it)) == valueAddr)
            {
                this->erase(it);
                return true;
            }
            it++;
        }
        return false;
    };

    bool removeAll(bool deletePtr)
    {
        if (this->empty())
            return false;

        Iterator it = this->begin();
        while (it != this->end())
        {
            if (deletePtr)
            {
                delete *it;
                *it = nullptr;
            }
            it = this->erase(it);
        }
        return true;
    };

    template <typename TPropValue>
    T *first(T &instance, const TPropValue &(T::*getter)() const)
    {
        if (this->empty())
            return nullptr;
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && (instance.*getter)() == ((*it)->*getter)())
                return *it;
        }
        return nullptr;
    };

    template <typename TPropValue>
    T *first(T &instance, TPropValue (T::*getter)())
    {
        if (this->empty())
            return nullptr;
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && (instance.*getter)() == ((*it)->*getter)())
                return *it;
        }
        return nullptr;
    };

    template <typename TPropValue>
    T *first(T &instance, TPropValue &(T::*getter)())
    {
        if (this->empty())
            return nullptr;
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && (instance.*getter)() == ((*it)->*getter)())
                return *it;
        }
        return nullptr;
    };

    template <typename TPropValue>
    T *first(T &instance, TPropValue (T::*getter)() const)
    {
        if (this->empty())
            return nullptr;
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && (instance.*getter)() == ((*it)->*getter)())
                return *it;
        }
        return nullptr;
    };

    // template <typename TPropValue>
    // T *first(const TPropValue &(T::*getter)() const, const TPropValue value)
    // {
    //     if (this->empty())
    //         return nullptr;
    //     for (Iterator it = this->begin(); it != this->end(); it++)
    //     {
    //         if (*it && value == ((*it)->*getter)())
    //             return *it;
    //     }
    //     return nullptr;
    // };

    template <typename TPropValue>
    T *first(const TPropValue &(T::*getter)() const, TPropValue value)
    {
        if (this->empty())
            return nullptr;
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && value == ((*it)->*getter)())
                return *it;
        }
        return nullptr;
    };

    template <typename TPropValue>
    T *first(TPropValue (T::*getter)() const, TPropValue value)
    {
        if (this->empty())
            return nullptr;
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && value == ((*it)->*getter)())
                return *it;
        }
        return nullptr;
    };

    T *first(T &instance)
    {
        if (this->empty())
            return nullptr;
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && instance == *(*it))
                return *it;
        }
        return nullptr;
    };

    T *first(T *instance)
    {
        if (this->empty())
            return nullptr;
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && instance == (*it))
                return *it;
        }
        return nullptr;
    };

    template <typename TPropValue>
    Vec<T> where(T &instance, const TPropValue &(T::*getter)() const)
    {
        if (this->empty())
            return Vec<T>();
        Vec<T> vec = Vec<T>();
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && (instance.*getter)() == ((*it)->*getter)())
                vec.push_back(*it);
        }
        return vec;
    };

    template <typename TPropValue>
    Vec<T> where(T &instance, TPropValue (T::*getter)())
    {
        if (this->empty())
            return Vec<T>();
        Vec<T> vec = Vec<T>();
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && (instance.*getter)() == ((*it)->*getter)())
                vec.push_back(*it);
        }
        return vec;
    };

    template <typename TPropValue>
    Vec<T> where(T &instance, TPropValue &(T::*getter)())
    {
        if (this->empty())
            return Vec<T>();
        Vec<T> vec = Vec<T>();
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && (instance.*getter)() == ((*it)->*getter)())
                vec.push_back(*it);
        }
        return vec;
    };

    template <typename TPropValue>
    Vec<T> where(T &instance, TPropValue (T::*getter)() const)
    {
        if (this->empty())
            return Vec<T>();
        Vec<T> vec = Vec<T>();
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && (instance.*getter)() == ((*it)->*getter)())
                vec.push_back(*it);
        }
        return vec;
    };

    template <typename TPropValue>
    Vec<T> where(const TPropValue &(T::*getter)() const, TPropValue value)
    {
        if (this->empty())
            return Vec<T>();
        Vec<T> vec = Vec<T>();
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && value == ((*it)->*getter)())
                vec.push_back(*it);
        }
        return vec;
    };

    template <typename TPropValue>
    Vec<T> where(TPropValue (T::*getter)() const, TPropValue value)
    {
        if (this->empty())
            return Vec<T>();
        Vec<T> vec = Vec<T>();
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && value == ((*it)->*getter)())
                vec.push_back(*it);
        }
        return vec;
    };

    Vec<T> where(T &instance)
    {
        if (this->empty())
            return Vec<T>();
        Vec<T> vec = Vec<T>();
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && instance == *(*it))
                vec.push_back(*it);
        }
        return vec;
    };

    Vec<T> where(T *instance)
    {
        if (this->empty())
            return Vec<T>();
        Vec<T> vec = Vec<T>();
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && instance == (*it))
                vec.push_back(*it);
        }
        return vec;
    };

    //////////////////////////////////////////////////////////////////////////

    template <typename TPropValue>
    bool removeWhere(TPropValue (T::*getter)() const, TPropValue value, bool deletePtr)
    {
        if (this->empty())
            return false;
        bool removed = false;
        Iterator it = this->begin();
        while (it != this->end())
        {
            if (*it && value == ((*it)->*getter)())
            {
                if (deletePtr)
                {
                    delete *it;
                    *it = nullptr;
                }
                it = this->erase(it);
                removed = true;
            }
            else
                it++;
        }
        return removed;
    };

    template <typename TPropValue>
    bool removeWhere(const TPropValue &(T::*getter)() const, const TPropValue &value, bool deletePtr)
    {
        if (this->empty())
            return false;
        bool removed = false;
        Iterator it = this->begin();
        while (it != this->end())
        {
            if (*it && value == ((*it)->*getter)())
            {
                if (deletePtr)
                {
                    delete *it;
                    *it = nullptr;
                }
                it = this->erase(it);
                removed = true;
            }
            else
                it++;
        }
        return removed;
    };

    template <typename TPropValue>
    bool removeWhere(T &instance, const TPropValue &(T::*getter)() const, bool deletePtr)
    {
        if (this->empty())
            return false;
        bool removed = false;
        Iterator it = this->begin();
        while (it != this->end())
        {
            if (*it && (instance.*getter)() == ((*it)->*getter)())
            {
                if (deletePtr)
                {
                    delete *it;
                    *it = nullptr;
                }
                it = this->erase(it);
                removed = true;
            }
            else
                it++;
        }
        return removed;
    };

    template <typename TPropValue>
    bool removeWhere(T &instance, TPropValue (T::*getter)(), bool deletePtr)
    {
        if (this->empty())
            return false;
        bool removed = false;
        Iterator it = this->begin();
        while (it != this->end())
        {
            if (*it && (instance.*getter)() == ((*it)->*getter)())
            {
                if (deletePtr)
                {
                    delete *it;
                    *it = nullptr;
                }
                it = this->erase(it);
                removed = true;
            }
            else
                it++;
        }
        return removed;
    };

    template <typename TPropValue>
    bool removeWhere(T &instance, TPropValue &(T::*getter)(), bool deletePtr)
    {
        if (this->empty())
            return false;
        bool removed = false;
        Iterator it = this->begin();
        while (it != this->end())
        {
            if (*it && (instance.*getter)() == ((*it)->*getter)())
            {
                if (deletePtr)
                {
                    delete *it;
                    *it = nullptr;
                }
                it = this->erase(it);
                removed = true;
            }
            else
                it++;
        }
        return removed;
    };

    template <typename TPropValue>
    bool removeWhere(T &instance, TPropValue (T::*getter)() const, bool deletePtr)
    {
        if (this->empty())
            return false;
        bool removed = false;
        Iterator it = this->begin();
        while (it != this->end())
        {
            if (*it && (instance.*getter)() == ((*it)->*getter)())
            {
                if (deletePtr)
                {
                    delete *it;
                    *it = nullptr;
                }
                it = this->erase(it);
                removed = true;
            }
            else
                it++;
        }
        return removed;
    };

    bool removeWhere(T &instance, bool deletePtr)
    {
        if (this->empty())
            return false;
        bool removed = false;
        Iterator it = this->begin();
        while (it != this->end())
        {
            if (*it && instance == *(*it))
            {
                if (deletePtr)
                {
                    delete *it;
                    *it = nullptr;
                }
                it = this->erase(it);
                removed = true;
            }
            else
                it++;
        }
        return removed;
    };

    bool removeWhere(T *instance, bool deletePtr)
    {
        if (this->empty())
            return false;
        bool removed = false;
        Iterator it = this->begin();
        while (it != this->end())
        {
            if (*it && instance == (*it))
            {
                if (deletePtr)
                {
                    delete *it;
                    *it = nullptr;
                }
                it = this->erase(it);
                removed = true;
            }
            else
                it++;
        }
        return removed;
    };

    //////////////////////////////////////////////////////////////////////////

    template <typename TPropValue>
    bool exist(TPropValue (T::*getter)() const, TPropValue value)
    {
        if (this->empty())
            return false;
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && value == ((*it)->*getter)())
                return true;
        }
        return false;
    };

    template <typename TPropValue>
    bool exist(const TPropValue &(T::*getter)() const, const TPropValue &value)
    {
        if (this->empty())
            return false;
        for (Iterator it = this->begin(); it != this->end(); it++)
        {
            if (*it && value == ((*it)->*getter)())
                return true;
        }
        return false;
    };

    template <typename TPropValue>
    bool exist(T &instance, const TPropValue &(T::*getter)() const)
    {
        if (this->empty())
            return false;
        Iterator it = this->begin();
        while (it != this->end())
        {
            if (*it && (instance.*getter)() == ((*it)->*getter)())
            {
                it = this->erase(it);
                return true;
            }
            it++;
        }
        return false;
    };

    template <typename TPropValue>
    bool exist(T &instance, TPropValue (T::*getter)())
    {
        if (this->empty())
            return false;
        Iterator it = this->begin();
        while (it != this->end())
        {
            if (*it && (instance.*getter)() == ((*it)->*getter)())
            {
                it = this->erase(it);
                return true;
            }
            it++;
        }
        return false;
    };

    template <typename TPropValue>
    bool exist(T &instance, TPropValue &(T::*getter)())
    {
        if (this->empty())
            return false;
        Iterator it = this->begin();
        while (it != this->end())
        {
            if (*it && (instance.*getter)() == ((*it)->*getter)())
            {
                it = this->erase(it);
                return true;
            }
            it++;
        }
        return false;
    };

    template <typename TPropValue>
    bool exist(T &instance, TPropValue (T::*getter)() const)
    {
        if (this->empty())
            return false;
        Iterator it = this->begin();
        while (it != this->end())
        {
            if (*it && (instance.*getter)() == ((*it)->*getter)())
            {
                it = this->erase(it);
                return true;
            }
            it++;
        }
        return false;
    };

    bool exist(T &instance)
    {
        if (this->empty())
            return false;
        Iterator it = this->begin();
        while (it != this->end())
        {
            if (*it && instance == *(*it))
            {
                it = this->erase(it);
                return true;
            }
            it++;
        }
        return false;
    };

    bool exist(T *instance)
    {
        if (this->empty())
            return false;
        Iterator it = this->begin();
        while (it != this->end())
        {
            if (*it && instance == (*it))
            {
                it = this->erase(it);
                return true;
            }
            it++;
        }
        return false;
    };
};