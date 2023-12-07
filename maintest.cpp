// #include <iostream>
// #include <vector>

// template <typename T>
// class Vec : public std::vector<T *>
// {
// public:
//     typedef typename std::vector<T *>::iterator Iterator;
//     Vec() : std::vector<T *>() {}

//     void addData(T *data)
//     {
//         this->push_back(data);
//     };

//     template <typename TReturn>
//     Vec<TReturn> select(TReturn *(T::*getter)())
//     {
//         if (this->empty())
//             return Vec<TReturn>();
//         Vec<TReturn> vec = Vec<TReturn>();
//         for (Iterator it = this->begin(); it != this->end(); it++)
//         {
//             if (*it)
//                 vec.push_back(((*it)->*getter)());
//         }
//         return vec;
//     };

//     bool remove(T *valueAddr)
//     {
//         if (this->empty() || valueAddr == NULL)
//             return false;
//         for (Iterator it = this->begin(); it != this->end(); it++)
//         {
//             if (*it == valueAddr)
//             {
//                 this->erase(it);
//                 return true;
//             }
//         }
//         return false;
//     };

//     bool remove(T &valueAddr)
//     {
//         if (this->empty())
//             return false;
//         for (Iterator it = this->begin(); it != this->end(); it++)
//         {
//             if ((*(*it)) == valueAddr)
//             {
//                 this->erase(it);
//                 return true;
//             }
//         }
//         return false;
//     };

//     template <typename TPropValue>
//     Vec<T> where(T &instance, const TPropValue &(T::*getter)() const)
//     {
//         if (this->empty())
//             return Vec<T>();
//         Vec<T> vec = Vec<T>();
//         for (Iterator it = this->begin(); it != this->end(); it++)
//         {
//             if (*it && (instance.*getter)() == ((*it)->*getter)())
//                 vec.push_back(*it);
//         }
//         return vec;
//     };

//     template <typename TPropValue>
//     Vec<T> where(T &instance, TPropValue (T::*getter)())
//     {
//         if (this->empty())
//             return Vec<T>();
//         Vec<T> vec = Vec<T>();
//         for (Iterator it = this->begin(); it != this->end(); it++)
//         {
//             if (*it && (instance.*getter)() == ((*it)->*getter)())
//                 vec.push_back(*it);
//         }
//         return vec;
//     };

//     template <typename TPropValue>
//     Vec<T> where(T &instance, TPropValue &(T::*getter)())
//     {
//         if (this->empty())
//             return Vec<T>();
//         Vec<T> vec = Vec<T>();
//         for (Iterator it = this->begin(); it != this->end(); it++)
//         {
//             if (*it && (instance.*getter)() == ((*it)->*getter)())
//                 vec.push_back(*it);
//         }
//         return vec;
//     };

//     template <typename TPropValue>
//     Vec<T> where(T &instance, TPropValue (T::*getter)() const)
//     {
//         if (this->empty())
//             return Vec<T>();
//         Vec<T> vec = Vec<T>();
//         for (Iterator it = this->begin(); it != this->end(); it++)
//         {
//             if (*it && (instance.*getter)() == ((*it)->*getter)())
//                 vec.push_back(*it);
//         }
//         return vec;
//     };

//     Vec<T> where(T &instance)
//     {
//         if (this->empty())
//             return Vec<T>();
//         Vec<T> vec = Vec<T>();
//         for (Iterator it = this->begin(); it != this->end(); it++)
//         {
//             if (*it && instance == *(*it))
//                 vec.push_back(*it);
//         }
//         return vec;
//     };

//     Vec<T> where(T *instance)
//     {
//         if (this->empty())
//             return Vec<T>();
//         Vec<T> vec = Vec<T>();
//         for (Iterator it = this->begin(); it != this->end(); it++)
//         {
//             if (*it && instance == (*it))
//                 vec.push_back(*it);
//         }
//         return vec;
//     };

//     //////////////////////////////////////////////////////////////////////////

//     template <typename TPropValue>
//     bool removeWhere(T &instance, const TPropValue &(T::*getter)() const)
//     {
//         if (this->empty())
//             return false;
//         bool removed = false;
//         Iterator it = this->begin();
//         while (it != this->end())
//         {
//             if (*it && (instance.*getter)() == ((*it)->*getter)())
//             {
//                 this->erase(it);
//                 removed = true;
//             }
//             else
//                 it++;
//         }
//         return removed;
//     };

//     template <typename TPropValue>
//     bool removeWhere(T &instance, TPropValue (T::*getter)())
//     {
//         if (this->empty())
//             return false;
//         bool removed = false;
//         Iterator it = this->begin();
//         while (it != this->end())
//         {
//             if (*it && (instance.*getter)() == ((*it)->*getter)())
//             {
//                 this->erase(it);
//                 removed = true;
//             }
//             else
//                 it++;
//         }
//         return removed;
//     };

//     template <typename TPropValue>
//     bool removeWhere(T &instance, TPropValue &(T::*getter)())
//     {
//         if (this->empty())
//             return false;
//         bool removed = false;
//         Iterator it = this->begin();
//         while (it != this->end())
//         {
//             if (*it && (instance.*getter)() == ((*it)->*getter)())
//             {
//                 this->erase(it);
//                 removed = true;
//             }
//             else
//                 it++;
//         }
//         return removed;
//     };

//     template <typename TPropValue>
//     bool removeWhere(T &instance, TPropValue (T::*getter)() const)
//     {
//         if (this->empty())
//             return false;
//         bool removed = false;
//         Iterator it = this->begin();
//         while (it != this->end())
//         {
//             if (*it && (instance.*getter)() == ((*it)->*getter)())
//             {
//                 this->erase(it);
//                 removed = true;
//             }
//             else
//                 it++;
//         }
//         return removed;
//     };

//     bool removeWhere(T &instance)
//     {
//         if (this->empty())
//             return false;
//         bool removed = false;
//         Iterator it = this->begin();
//         while (it != this->end())
//         {
//             if (*it && instance == *(*it))
//             {
//                 this->erase(it);
//                 removed = true;
//             }
//             else
//                 it++;
//         }
//         return removed;
//     };

//     bool removeWhere(T *instance)
//     {
//         if (this->empty())
//             return false;
//         bool removed = false;
//         Iterator it = this->begin();
//         while (it != this->end())
//         {
//             if (*it && instance == (*it))
//             {
//                 this->erase(it);
//                 removed = true;
//             }
//             else
//                 it++;
//         }
//         return removed;
//     };

//     //////////////////////////////////////////////////////////////////////////
//     template <typename TPropValue>
//     bool exist(T &instance, const TPropValue &(T::*getter)() const)
//     {
//         if (this->empty())
//             return false;
//         Iterator it = this->begin();
//         while (it != this->end())
//         {
//             if (*it && (instance.*getter)() == ((*it)->*getter)())
//             {
//                 this->erase(it);
//                 return true;
//             }
//             it++;
//         }
//         return false;
//     };

//     template <typename TPropValue>
//     bool exist(T &instance, TPropValue (T::*getter)())
//     {
//         if (this->empty())
//             return false;
//         Iterator it = this->begin();
//         while (it != this->end())
//         {
//             if (*it && (instance.*getter)() == ((*it)->*getter)())
//             {
//                 this->erase(it);
//                 return true;
//             }
//             it++;
//         }
//         return false;
//     };

//     template <typename TPropValue>
//     bool exist(T &instance, TPropValue &(T::*getter)())
//     {
//         if (this->empty())
//             return false;
//         Iterator it = this->begin();
//         while (it != this->end())
//         {
//             if (*it && (instance.*getter)() == ((*it)->*getter)())
//             {
//                 this->erase(it);
//                 return true;
//             }
//             it++;
//         }
//         return false;
//     };

//     template <typename TPropValue>
//     bool exist(T &instance, TPropValue (T::*getter)() const)
//     {
//         if (this->empty())
//             return false;
//         Iterator it = this->begin();
//         while (it != this->end())
//         {
//             if (*it && (instance.*getter)() == ((*it)->*getter)())
//             {
//                 this->erase(it);
//                 return true;
//             }
//             it++;
//         }
//         return false;
//     };

//     bool exist(T &instance)
//     {
//         if (this->empty())
//             return false;
//         Iterator it = this->begin();
//         while (it != this->end())
//         {
//             if (*it && instance == *(*it))
//             {
//                 this->erase(it);
//                 return true;
//             }
//             it++;
//         }
//         return false;
//     };

//     bool exist(T *instance)
//     {
//         if (this->empty())
//             return false;
//         Iterator it = this->begin();
//         while (it != this->end())
//         {
//             if (*it && instance == (*it))
//             {
//                 this->erase(it);
//                 return true;
//             }
//             it++;
//         }
//         return false;
//     };
// };

// // Example usage
// class Example2
// {
// public:
//     int value;

//     Example2(int val) : value(val) {}

//     int getValue() const
//     {
//         return value;
//     }
// };

// // Example usage
// class Example
// {
// public:
//     int value;
//     std::string str;

//     Example(int val) : value(val)
//     {
//         str = "Mystring";
//     }

//     bool operator==(const Example &cmp) const
//     {
//         return this->getValue() == cmp.getValue() && this->getConstStrConst() == cmp.getConstStrConst();
//     }
//     bool operator!=(const Example &cmp) const
//     {
//         return this->getValue() == cmp.getValue() && this->getConstStrConst() == cmp.getConstStrConst();
//     }

//     Example2 *getExample2()
//     {
//         return new Example2(this->getValue() + 100);
//     }

//     int getValue() const
//     {
//         return value;
//     }

//     const std::string &getConstStrConst() const
//     {
//         return str;
//     }

//     std::string &getStr()
//     {
//         return str;
//     }

//     std::string getStrCopy()
//     {
//         return str;
//     }
// };

// int main()
// {
//     Vec<Example> exampleVec;

//     Example *example1 = new Example(41);
//     Example *example2 = new Example(42);
//     exampleVec.addData(example1);
//     exampleVec.addData(example2);

//     Example *example3 = new Example(43);

//     // std::cout << "Should be in the vector, should be 1 : " << exampleVec.isIn(*example1, &Example::getValue) << std::endl;
//     // std::cout << "Should not be in the vector, should be 0 : " << exampleVec.isIn(*example3, &Example::getValue) << std::endl;

//     Vec<Example2> sel = exampleVec.select(&Example::getExample2);

//     // std::cout << "does it exist, should be 1 is: " << exampleVec.existByAddress(example1) << std::endl;
//     // std::cout << "does it exist, should be 0 is: " << exampleVec.existByAddress(NULL) << std::endl;
//     // std::cout << "does it exist, should be 0 is: " << exampleVec.existByAddress(example3) << std::endl;

//     // std::cout << "does it exist, should be 1 is: " << exampleVec.exist(*example1) << std::endl;
//     // std::cout << "does it exist, should be 0 is: " << exampleVec.exist(*example3) << std::endl;

//     // std::cout << "does it exist, should be 1 is: " << exampleVec.exist(*example1) << std::endl;
//     // std::cout << "does it exist, should be 0 is: " << exampleVec.exist(*example3) << std::endl;

//     std::cout << "1 where test sould be 1 is : " << exampleVec.where(*example1).size() << std::endl;
//     std::cout << "2 where test sould be 1 is : " << exampleVec.where(*example2).size() << std::endl;
//     std::cout << "3 where test sould be 0 is : " << exampleVec.where(*example3).size() << std::endl;
//     std::cout << "4 where test sould be 1 is : " << exampleVec.where(example1).size() << std::endl;
//     std::cout << "4 where test sould be 0 is : " << exampleVec.where(example3).size() << std::endl;
//     std::cout << "5 where test sould be 2 is : " << exampleVec.where(*example1, &Example::getStr).size() << std::endl;
//     std::cout << "6 where test sould be 2 is : " << exampleVec.where(*example1, &Example::getStr).size() << std::endl;
//     std::cout << "7 where test sould be 2 is : " << exampleVec.where(*example1, &Example::getConstStrConst).size() << std::endl;
//     std::cout << "8 where test sould be 2 is : " << exampleVec.where(*example1, &Example::getStrCopy).size() << std::endl;
//     std::cout << "9 where test sould be 0 is : " << exampleVec.where(*example3, &Example::getStrCopy).size() << std::endl;
//     std::cout << "10 where test sould be 1 is : " << exampleVec.where(*example1, &Example::getValue).size() << std::endl;
//     std::cout << "11 where test sould be 0 is : " << exampleVec.where(*example3, &Example::getValue).size() << std::endl
//               << std::endl;

//     std::cout << "1 removeWhere test sould be 1 1 is : " << exampleVec.removeWhere(*example1) << " " << exampleVec.size() << std::endl;
//     exampleVec.addData(example1);
//     std::cout << "2 removeWhere test sould be 1 1 is : " << exampleVec.removeWhere(*example2) << "  " << exampleVec.size() << std::endl;
//     exampleVec.addData(example2);
//     std::cout << "3 removeWhere test sould be 0 2 is : " << exampleVec.removeWhere(*example3) << "  " << exampleVec.size() << std::endl;
//     std::cout << "4 removeWhere test sould be 1 1 is : " << exampleVec.removeWhere(example1) << "  " << exampleVec.size() << std::endl;
//     exampleVec.addData(example1);
//     std::cout << "4 removeWhere test sould be 0 2 is : " << exampleVec.removeWhere(example3) << "  " << exampleVec.size() << std::endl;
//     std::cout << "5 removeWhere test sould be 1 0 is : " << exampleVec.removeWhere(*example1, &Example::getStr) << "  " << exampleVec.size() << std::endl;
//     exampleVec.addData(example1);
//     exampleVec.addData(example2);
//     std::cout << "6 removeWhere test sould be 1 0 is : " << exampleVec.removeWhere(*example1, &Example::getStr) << "  " << exampleVec.size() << std::endl;
//     exampleVec.addData(example1);
//     exampleVec.addData(example2);
//     std::cout << "7 removeWhere test sould be 1 0 is : " << exampleVec.removeWhere(*example1, &Example::getConstStrConst) << "  " << exampleVec.size() << std::endl;
//     exampleVec.addData(example1);
//     exampleVec.addData(example2);
//     std::cout << "8 removeWhere test sould be 1 0 is : " << exampleVec.removeWhere(*example1, &Example::getStrCopy) << "  " << exampleVec.size() << std::endl;
//     exampleVec.addData(example1);
//     exampleVec.addData(example2);
//     std::cout << "9 removeWhere test sould be 1 0 is : " << exampleVec.removeWhere(*example3, &Example::getStrCopy) << "  " << exampleVec.size() << std::endl;
//     exampleVec.addData(example1);
//     exampleVec.addData(example2);
//     std::cout << "10 removeWhere test sould be 1 1 is : " << exampleVec.removeWhere(*example1, &Example::getValue) << "  " << exampleVec.size() << std::endl;
//     exampleVec.addData(example1);
//     std::cout << "11 removeWhere test sould be 0 2 is : " << exampleVec.removeWhere(*example3, &Example::getValue) << "  " << exampleVec.size() << std::endl
//               << std::endl;

//     exampleVec.remove(example2);
//     exampleVec.remove(example2);
//     exampleVec.remove(*example1);

//     std::cout << "vector count should be 0 is: " << exampleVec.size() << std::endl;
//     return 0;
// }
