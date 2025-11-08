#include <algorithm>
#include <stdexcept>
#include <impulses/keybindstation.h>

#include "thirdparty/containers.h"

namespace m3d
{
    KeysSet::KeysSet()
    {
    }

    KeysSet::KeysSet(int key)
    {
        m_set.insert(key);
    }

    void KeysSet::clear()
    {
        m_set.clear();
    }

    unsigned KeysSet::size() const
    {
        return m_set.size();
    }

    bool KeysSet::empty() const
    {
        return m_set.empty();
    }

    KeysSet& KeysSet::operator-=(int key)
    {
        m_set.erase(key);
        return *this;
    }

    KeysSet& KeysSet::operator+=(int k)
    {
        m_set.insert(k);
        return *this;
    }

    bool KeysSet::IsThere(int k)
    {
        return m_set.find(k) != m_set.end();
    }

    std::set<int>::iterator KeysSet::begin()
    {
        return m_set.begin();
    }

    std::set<int>::iterator KeysSet::end()
    {
        return m_set.end();
    }

    std::set<int>::const_iterator KeysSet::begin() const
    {
        return m_set.begin();
    }

    std::set<int>::const_iterator KeysSet::end() const
    {
        return m_set.end();
    }

    std::set<int>::reverse_iterator KeysSet::rbegin() const
    {
        return m_set.rbegin();
    }

    std::set<int>::reverse_iterator KeysSet::rend() const
    {
        return m_set.rend();
    }

    KeyBindStation::KeyBindStation()
    {
        m_longestComboLen = 0;
    }

    KeyBindStation::~KeyBindStation()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void KeyBindStation::UnbindImpulse(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void KeyBindStation::UnbindAll()
    {
        m_bindings.clear();
    }

    void KeyBindStation::UnbindImpulseFromKeyset(int, KeysSet const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int KeyBindStation::FindImpulseByLongestSetPossible(m3d::KeysSet const& setToSearchFrom, int keyToSearchWith, KeysSet& impulseSet)
    {
        // TODO: check this!!!!!
        impulseSet.clear();
        if (keyToSearchWith == -1)
        {
            for (auto it = setToSearchFrom.rbegin(); it != setToSearchFrom.rend(); ++it)
            {
                KeysSet subset = setToSearchFrom;
                subset -= *it;

                auto* bindKey = FindImpulseBySet_r(subset, *it);

                if (bindKey)
                {
                    impulseSet = *m_ks;
                    return bindKey->m_impulse;
                }
            }
        }

        KeysSet subset = setToSearchFrom;
        subset -= keyToSearchWith;

        auto* bindKey = FindImpulseBySet_r(subset, keyToSearchWith);
        if (bindKey)
        {
            impulseSet = *m_ks;
            return bindKey->m_impulse;
        }

        return -1;
    }

    KeyBindStation::BindKey* KeyBindStation::GetBindByKey(KeysSet const& ks)
    {
        //TODO: check this!!!!
        for (auto& bind : m_bindings)
        {
            auto it = std::find(bind.m_keys.begin(), bind.m_keys.end(), ks);
            if (it != bind.m_keys.end())
            {
                m_ks = &*it;
                return &bind;
            }
        }
        return nullptr;
    }

    void KeyBindStation::BindKeyToImpulse(KeysSet const& ks, int imp)
    {
        auto bind = GetBindByKey(ks);
        for (m_lastIndex = m_bindings.begin(); m_lastIndex != m_bindings.end(); ++m_lastIndex)
        {
	        if (imp == m_lastIndex->m_impulse)
	        {
                break;
	        }
        }
        if (bind && bind->m_impulse != imp)
        {
            auto it = std::find(bind->m_keys.begin(), bind->m_keys.end(), ks);
            bind->m_keys.erase(it);
            //std::copy(it + 1, bind->m_keys.end(), it);
            //bind->m_keys.pop_back();
        }
        if (m_lastIndex == m_bindings.end())
        {
            BindKey bindKey;
            m_bindings.push_back(bindKey);
            m_lastIndex = m_bindings.end() - 1;
            m_lastIndex->m_impulse = imp;
        }
        m_lastIndex->m_keys.push_back(ks);
        auto ksSize = ks.size();
        if (ksSize > m_longestComboLen)
        {
            m_longestComboLen = ksSize;
        }
    }

    KeyBindStation::BindKey* KeyBindStation::GetBindByImpulse(int)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    KeyBindStation::BindKey* KeyBindStation::FindImpulseBySet_r(KeysSet const& ks, int key)
    {
        // TODO: generated code
        size_t keySetSize = ks.size();

        // Check if key set exceeds maximum combination length
        if (keySetSize > m_longestComboLen)
        {
            // Try excluding each key one by one and search recursively
            for (auto it = ks.begin(); it != ks.end(); ++it) {
                int excludedKey = *it;

                // Create set without the excluded key
                KeysSet reducedSet = ks;
                reducedSet -= excludedKey;

                // Recursively search with reduced set
                BindKey* binding = FindImpulseBySet_r(reducedSet, key);
                if (binding) {
                    return binding;
                }
            }
            return nullptr;
        }

        // Handle small key sets (size <= 1)
        if (keySetSize <= 1)
        {
            // Try combination of existing keys + new key
            KeysSet combinedSet = ks;
            combinedSet += key;
            BindKey* binding = GetBindByKey(combinedSet);

            if (binding) {
                return binding;
            }

            KeysSet singleKeySet(key);
            binding = GetBindByKey(singleKeySet);
            return binding;
        }

        // For medium-sized key sets, try replacing each key with the new key
        for (auto it = ks.begin(); it != ks.end(); ++it) {
            int existingKey = *it;

            // Create set with one key replaced by the new key
            KeysSet modifiedSet = ks;
            modifiedSet -= existingKey;
            modifiedSet += key;

            BindKey* binding = GetBindByKey(modifiedSet);
            if (binding) {
                return binding;
            }
        }

        // If replacement strategy fails, try the exclusion strategy
        for (auto it = ks.begin(); it != ks.end(); ++it) {
            int excludedKey = *it;

            // Create set without the excluded key
            KeysSet reducedSet = ks;
            reducedSet -= excludedKey;

            // Recursively search with reduced set
            BindKey* binding = FindImpulseBySet_r(reducedSet, key);
            if (binding) {
                return binding;
            }
        }

        return nullptr;
    }

    bool operator==(const KeysSet& lhd, const KeysSet& rhd)
    {
        if (lhd.size() == rhd.size())
        {
            return std::mismatch(lhd.m_set.begin(), lhd.m_set.end(), rhd.m_set.begin()).first == lhd.m_set.end();
        }
        return false;
    }

    KeysSet operator-(const KeysSet& lhd, const KeysSet& rhd)
    {
        auto res = lhd;
        res.m_set.erase(rhd.begin(), rhd.end());
        return res;
    }
}
