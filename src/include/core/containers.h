#pragma once
#include <map>
#include <stdexcept>
#include <unordered_map>
#include <core/stringm3d.h>

namespace m3d
{
    template<class T>
    class CStrHash
    {
    public:
        ~CStrHash()
        {
            throw std::logic_error("Not implemented");
        }

        void add(CStr const& key, T value)
        {
            m_hash[key] = value;
        }

        
        bool get(CStr const&, T&) const
        {
            throw std::logic_error("Not implemented");
        }

        void clear()
        {
            throw std::logic_error("Not implemented");
        }

    private:
        std::map<CStr, T> m_hash;
    };

    template <class T>
    class CIntHash
    {
    public:
        ~CIntHash()
        {
            throw std::logic_error("Not implemented");
        }

        void removeByKey(unsigned int)
        {
            throw std::logic_error("Not implemented");
        }

        void clear()
        {
            throw std::logic_error("Not implemented");
        }

        void addValueByKey(unsigned int, T const&)
        {
            throw std::logic_error("Not implemented");
        }

        bool getValueByKey(unsigned int, T&) const
        {
            throw std::logic_error("Not implemented");
        }

    private:
        std::unordered_map<unsigned int, T> m_hash;
    };
}
