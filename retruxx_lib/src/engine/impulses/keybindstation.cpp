#include <algorithm>
#include <stdexcept>
#include <impulses/keybindstation.h>

namespace m3d
{
    KeysSet::KeysSet()
    {
    }

    KeysSet::KeysSet(int)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    KeysSet& KeysSet::operator-=(int)
    {
        throw std::logic_error("Not implemented");
    }

    KeysSet& KeysSet::operator+=(int k)
    {
        m_set.insert(k);
        return *this;
    }

    bool KeysSet::IsThere(int)
    {
        throw std::logic_error("Not implemented");
    }

    KeyBindStation::KeyBindStation()
    {
    }

    KeyBindStation::~KeyBindStation()
    {
        throw std::logic_error("Not implemented");
    }

    void KeyBindStation::UnbindImpulse(int)
    {
        throw std::logic_error("Not implemented");
    }

    void KeyBindStation::UnbindAll()
    {
        m_bindings.clear();
    }

    void KeyBindStation::UnbindImpulseFromKeyset(int, KeysSet const&)
    {
        throw std::logic_error("Not implemented");
    }

    int KeyBindStation::FindImpulseByLongestSetPossible(m3d::KeysSet const&, int, KeysSet&)
    {
        throw std::logic_error("Not implemented");
    }

    KeyBindStation::BindKey* KeyBindStation::GetBindByKey(KeysSet const& ks)
    {
        //TODO: check this!!!!
        for (auto& bind : m_bindings)
        {
            auto it = std::find(bind.m_keys.begin(), bind.m_keys.end(), ks);
            if (it != bind.m_keys.end())
            {
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
        throw std::logic_error("Not implemented");
    }

    KeyBindStation::BindKey* KeyBindStation::FindImpulseBySet_r(KeysSet const&, int)
    {
        throw std::logic_error("Not implemented");
    }

    bool operator==(const KeysSet& lhd, const KeysSet& rhd)
    {
        if (lhd.size() == rhd.size())
        {
            return std::mismatch(lhd.m_set.begin(), lhd.m_set.end(), rhd.m_set.begin()).first == lhd.m_set.end();
        }
        return false;
    }
}
