#pragma once
#include "retruxx/common.h"
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

        void clear()
        {
            m_hash.clear();
        }

        void add(CStr const& key, T value)
        {
            m_hash[key] = value;
        }
        
        bool get(CStr const& key, T& v) const
        {
            auto it = m_hash.find(key);
            if (it != m_hash.end())
            {
                v = it->second;
                return true;
            }
            return false;
        }

        void remove(const CStr&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        using tStrHash = retruxx::map<CStr, T>;
        class tStrHashIt;
        class tConstStrHashIt;

    private:
        /* 0x0000 */ tStrHash m_hash;
    }; /* size: 0x000c */

    template <class T>
    class CIntHash
    {
    public:
        ~CIntHash()
        {
        }

        void clear()
        {
            m_hash.clear();
        }

        void removeByKey(unsigned int key)
        {
            m_hash.erase(key);
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

        void addValueByKey(unsigned int key, T const& val)
        {
            m_hash.insert(retruxx::pair<unsigned int, T>(key, val));
        }

        //using tHashFunction = stdext::hash_compare<unsigned int, std::less<unsigned int> >;
        using tHash = retruxx::hash_map<unsigned int, T>;
        class tHashIt;
        class tConstHashIt;

    private:
        /* 0x0000 */ tHash m_hash;
    }; /* size: 0x0028 */
}
