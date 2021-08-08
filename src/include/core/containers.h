#pragma once
#include <map>
#include <unordered_map>
#include <core/stringm3d.h>

namespace m3d
{
    template<class T>
    class CStrHash
    {
    public:
        ~CStrHash();
        void add(CStr const&, T);
        bool get(CStr const&, T&) const;
        void clear();

    private:
        std::map<CStr, T> m_hash;
    };

    template <class T>
    class CIntHash
    {
    public:
        ~CIntHash();
        void removeByKey(unsigned int);
        void clear();
        void addValueByKey(unsigned int, T const&);
        bool getValueByKey(unsigned int, T&) const;

    private:
        std::unordered_map<unsigned int, T> m_hash;
    };
}
