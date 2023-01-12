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
        }

        void add(CStr const& key, T value)
        {
            m_hash[key] = value;
        }

        
        bool get(CStr const& key, T& v) const
        {
            auto it = m_hash.find(key);
            if (it != end(m_hash))
            {
                v = it->second;
                return true;
            }
            return false;
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
        }

        void removeByKey(unsigned int key)
        {
            m_hash.erase(key);
        }

        void clear()
        {
            throw std::logic_error("Not implemented");
        }

        void addValueByKey(unsigned int key, T const& val)
        {
            m_hash.emplace(key, val);
        }

        bool getValueByKey(unsigned int key, T& v) const
        {
            auto it = m_hash.find(key);
            if (it == m_hash.end())
            {
                return false;
            }
            v = it->second;
            return true;
        }

    private:
        std::unordered_map<unsigned int, T> m_hash;
    };
}
