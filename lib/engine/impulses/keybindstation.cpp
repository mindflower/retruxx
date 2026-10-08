#include <algorithm>
#include <stdexcept>
#include <impulses/keybindstation.h>

#include "retruxx/common.h"

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
        // RVA 0x599790
        // m_ks points into m_bindings and is not owned.
    }

    void KeyBindStation::UnbindImpulse(int imp)
    {
        // RVA 0x7598D0 - removes the first binding of the impulse; m_lastIndex is left at the removed position, or at
        // the end when there is none.
        for (m_lastIndex = m_bindings.begin(); m_lastIndex != m_bindings.end(); ++m_lastIndex)
        {
            if (m_lastIndex->m_impulse == imp)
            {
                m_lastIndex = m_bindings.erase(m_lastIndex);
                return;
            }
        }
    }

    void KeyBindStation::UnbindAll()
    {
        m_bindings.clear();
    }

    void KeyBindStation::UnbindImpulseFromKeyset(int imp, KeysSet const& keyset)
    {
        // RVA 0x7590A0
        for (m_lastIndex = m_bindings.begin(); m_lastIndex != m_bindings.end(); ++m_lastIndex)
        {
            if (imp == m_lastIndex->m_impulse)
            {
                break;
            }
        }
        if (m_lastIndex == m_bindings.end())
        {
            return;
        }

        auto& keys = m_lastIndex->m_keys;
        for (auto it = keys.begin(); it != keys.end(); ++it)
        {
            // NOTE: the shipped code uses std::mismatch over the requested set only, so any stored set
            // that starts with the requested keys matches, even if it has more keys after them.
            // A stored set shorter than the requested one made the original read past its end; that
            // case is treated as a mismatch here.
            auto storedIt = it->begin();
            bool matches = true;
            for (int const key : keyset)
            {
                if (storedIt == it->end() || *storedIt != key)
                {
                    matches = false;
                    break;
                }
                ++storedIt;
            }

            if (matches)
            {
                keys.erase(it);
                return;
            }
        }
    }

    int KeyBindStation::FindImpulseByLongestSetPossible(m3d::KeysSet const& setToSearchFrom, int keyToSearchWith, KeysSet& impulseSet)
    {
        // RVA 0x758750
        impulseSet.clear();

        BindKey* bind = nullptr;
        if (keyToSearchWith == -1)
        {
            // Without a key to search with, each key of the set is tried in turn, from the highest down.
            for (auto it = setToSearchFrom.rbegin(); it != setToSearchFrom.rend() && !bind; ++it)
            {
                bind = FindImpulseBySet_r(setToSearchFrom - *it, *it);
            }
        }
        else
        {
            bind = FindImpulseBySet_r(setToSearchFrom - keyToSearchWith, keyToSearchWith);
        }

        if (!bind)
        {
            return -1;
        }
        impulseSet = *m_ks;
        return bind->m_impulse;
    }

    KeyBindStation::BindKey* KeyBindStation::GetBindByKey(KeysSet const& ks)
    {
        // RVA 0x758280 - m_lastIndex is left at the found binding (or at the end), and m_ks at the matching key set.
        for (m_lastIndex = m_bindings.begin(); m_lastIndex != m_bindings.end(); ++m_lastIndex)
        {
            for (auto& keys : m_lastIndex->m_keys)
            {
                if (keys == ks)
                {
                    m_ks = &keys;
                    return &*m_lastIndex;
                }
            }
        }
        return nullptr;
    }

    void KeyBindStation::BindKeyToImpulse(KeysSet const& ks, int imp)
    {
        // RVA 0x759A10
        BindKey* const oldBind = GetBindByKey(ks);
        BindKey* newBind = GetBindByImpulse(imp);  // the shipped code inlines this loop

        // A key set belongs to one impulse only: take it away from the impulse that had it.
        // NOTE: when the set is already bound to this same impulse, it is appended to its list a second time.
        if (oldBind && oldBind->m_impulse != imp)
        {
            oldBind->m_keys.erase(std::find(oldBind->m_keys.begin(), oldBind->m_keys.end(), ks));
        }

        if (!newBind)
        {
            // NOTE: m_lastIndex is left at the old end of m_bindings, as in the shipped code.
            m_bindings.push_back(BindKey());
            newBind = &m_bindings.back();
            newBind->m_impulse = imp;
        }
        newBind->m_keys.push_back(ks);

        if (ks.size() > m_longestComboLen)
        {
            m_longestComboLen = ks.size();
        }
    }

    KeyBindStation::BindKey* KeyBindStation::GetBindByImpulse(int imp)
    {
        // RVA 0x758020
        for (m_lastIndex = m_bindings.begin(); m_lastIndex != m_bindings.end(); ++m_lastIndex)
        {
            if (imp == m_lastIndex->m_impulse)
            {
                return &*m_lastIndex;
            }
        }
        return nullptr;
    }

    KeyBindStation::BindKey* KeyBindStation::FindImpulseBySet_r(KeysSet const& ks, int key)
    {
        // RVA 0x758540 - looks for a binding of `key` together with as many keys of `ks` as possible.
        if (ks.size() <= m_longestComboLen)
        {
            if (ks.size() <= 1)
            {
                KeysSet withKey = ks;  // operator+(KeysSet const&, int), RVA 0x7584D0
                withKey += key;
                BindKey* bind = GetBindByKey(withKey);
                if (!bind)
                {
                    bind = GetBindByKey(KeysSet(key));
                }
                return bind;
            }

            // Same size: swap one of the keys for `key`.
            for (int const replacedKey : ks)
            {
                KeysSet swapped = ks;
                swapped -= replacedKey;
                swapped += key;
                if (BindKey* bind = GetBindByKey(swapped))
                {
                    return bind;
                }
            }
        }

        // One key fewer, recursively.
        for (int const droppedKey : ks)
        {
            if (BindKey* bind = FindImpulseBySet_r(ks - droppedKey, key))
            {
                return bind;
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
        // RVA 0x5975A0 - every key of rhd is looked up and erased by value.
        auto res = lhd;
        for (int const key : rhd.m_set)
        {
            auto const it = res.m_set.find(key);
            if (it != res.m_set.end())
            {
                res.m_set.erase(it);
            }
        }
        return res;
    }

    KeysSet operator-(const KeysSet& lhd, int key)
    {
        // RVA 0x597520
        auto res = lhd;
        res.m_set.erase(key);
        return res;
    }
}
