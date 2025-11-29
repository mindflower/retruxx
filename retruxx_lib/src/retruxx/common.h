#pragma once
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
