#include <stdexcept>
#include <impulses/keybindstation.h>

namespace m3d
{
    KeysSet::KeysSet()
    {
        throw std::logic_error("Not implemented");
    }

    KeysSet::KeysSet(KeysSet const&)
    {
        throw std::logic_error("Not implemented");
    }

    KeysSet::KeysSet(int)
    {
        throw std::logic_error("Not implemented");
    }

    void KeysSet::clear()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned KeysSet::size() const
    {
        throw std::logic_error("Not implemented");
    }

    bool KeysSet::empty() const
    {
        throw std::logic_error("Not implemented");
    }

    KeysSet& KeysSet::operator-=(int)
    {
        throw std::logic_error("Not implemented");
    }

    KeysSet& KeysSet::operator+=(int)
    {
        throw std::logic_error("Not implemented");
    }

    bool KeysSet::IsThere(int)
    {
        throw std::logic_error("Not implemented");
    }

    KeyBindStation::KeyBindStation()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    void KeyBindStation::UnbindImpulseFromKeyset(int, KeysSet const&)
    {
        throw std::logic_error("Not implemented");
    }

    int KeyBindStation::FindImpulseByLongestSetPossible(m3d::KeysSet const&, int, KeysSet&)
    {
        throw std::logic_error("Not implemented");
    }

    KeyBindStation::BindKey* KeyBindStation::GetBindByKey(KeysSet const&)
    {
        throw std::logic_error("Not implemented");
    }

    void KeyBindStation::BindKeyToImpulse(KeysSet const&, int)
    {
        throw std::logic_error("Not implemented");
    }

    KeyBindStation::BindKey* KeyBindStation::GetBindByImpulse(int)
    {
        throw std::logic_error("Not implemented");
    }

    KeyBindStation::BindKey* KeyBindStation::FindImpulseBySet_r(KeysSet const&, int)
    {
        throw std::logic_error("Not implemented");
    }
}
