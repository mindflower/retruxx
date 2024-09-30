#pragma once
#ifdef RETRUXX_DLL

#include "allocator.h"
#include "stl/vector.hpp"
#include "stl/list.hpp"
#include "stl/set.hpp"
#include "stl/map.hpp"

namespace retruxx
{
    template <class T, class Al = retruxx::allocator<T>>
    using vector = oldstd::vector<T, Al>;

    template <class T, class Al = retruxx::allocator<T>>
    using list = oldstd::list<T, Al>;

    template<class T, class Comp = oldstd::less<T>, class Al = retruxx::allocator<T>>
    using set = oldstd::set<T, Comp, Al>;

    template<class Key, class Val, class Comp = oldstd::less<Key>, class Al = retruxx::allocator<oldstd::pair<const Key, Val> > >
    using map = oldstd::map<Key, Val, Comp, Al>;
}

#else //RETRUXX_DLL

#include <vector>
#include <list>
#include <set>
#include <map>

namespace retruxx
{
    template<typename... Args>
    using vector = std::vector<Args...>;

    template<typename... Args>
    using list = std::list<Args...>;

    template<typename... Args>
    using set = std::set<Args...>;

    template<typename... Args>
    using map = std::map<Args...>;
}

#endif //RETRUXX_DLL
