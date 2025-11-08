#pragma once
#ifdef RETRUXX_DLL

#include "allocator.h"
#include "stl/vector.hpp"
#include "stl/list.hpp"
#include "stl/set.hpp"
#include "stl/map.hpp"
#include "stl/hash_map.hpp"
#include "stl/deque.hpp"
#include "stl/stdexcept.hpp"

namespace retruxx
{
    using logic_error = oldstd::logic_error;

    template <class T, class Al = retruxx::allocator<T>>
    using vector = oldstd::vector<T, Al>;

    template <class T, class Al = retruxx::allocator<T>>
    using deque = oldstd::deque<T, Al>;

    template <class T, class Al = retruxx::allocator<T>>
    using list = oldstd::list<T, Al>;

    template<typename... Args>
    using pair = oldstd::pair<Args...>;

    template<typename... Args>
    using less = oldstd::less<Args...>;

    template<class T, class Comp = oldstd::less<T>, class Al = retruxx::allocator<T>>
    using set = oldstd::set<T, Comp, Al>;

    template<class Key, class Val, class Comp = oldstd::less<Key>, class Al = retruxx::allocator<oldstd::pair<const Key, Val>>>
    using map = oldstd::map<Key, Val, Comp, Al>;

    template<class Key, class Val, class Comp = oldstdext::hash_compare<Key, oldstd::less<Key>>, class Al = retruxx::allocator<oldstd::pair<const Key, Val> > >
    using hash_map = oldstdext::hash_map<Key, Val, Comp, Al>;

    using string = oldstd::string;

    template<typename... Args>
    using _Bidit = oldstd::_Bidit<Args...>;

    using bidirectional_iterator_tag = oldstd::bidirectional_iterator_tag;

    template<typename... Args>
    using binary_function = oldstd::binary_function<Args...>;
}

#else //RETRUXX_DLL

#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <stdexcept>
#include <string>
#include <stack>
#include <queue>

#define RETRUXX_NOT_IMPLEMENTED __debugbreak(); std::abort()

namespace retruxx
{
    using logic_error = std::logic_error;

    template<typename... Args>
    using allocator = std::allocator<Args...>;

    template<typename... Args>
    using vector = std::vector<Args...>;

    template<typename... Args>
    using deque = std::deque<Args...>;

    template<typename... Args>
    using list = std::list<Args...>;

    template<typename... Args>
    using pair = std::pair<Args...>;

    template<typename... Args>
    using less = std::less<Args...>;

    template<typename... Args>
    using set = std::set<Args...>;

    template<typename... Args>
    using map = std::map<Args...>;

    template<typename... Args>
    using hash_map = std::unordered_map<Args...>;

    using string = std::string;

    //template<typename... Args>
    //using _Bidit = std::_Bidit<Args...>;

    template<typename... Args>
    struct _Bidit {};

    using bidirectional_iterator_tag = std::bidirectional_iterator_tag;

    //template<typename... Args>
    //using binary_function = std::binary_function<Args...>;

    template<typename... Args>
    struct binary_function {};

    template<typename... Args>
    using stack = std::stack<Args...>;

    template<typename... Args>
    using queue = std::queue<Args...>;
}

#endif //RETRUXX_DLL
