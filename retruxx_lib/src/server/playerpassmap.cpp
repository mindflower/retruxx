#include "playerpassmap.h"

#include <stdexcept>

namespace ai
{
    bool PlayerPassMap::GetValue(unsigned, unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    void PlayerPassMap::Clear()
    {
        throw std::logic_error("Not implemented");
    }

    bool PlayerPassMap::IsEmpty() const
    {
        throw std::logic_error("Not implemented");
    }

    bool PlayerPassMap::SaveToBinaryFile(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    void PlayerPassMap::SetValue(unsigned, unsigned, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void PlayerPassMap::Create(unsigned, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void PlayerPassMap::Fill(bool)
    {
        throw std::logic_error("Not implemented");
    }

    PlayerPassMap::PlayerPassMap()
    {
    }

    bool PlayerPassMap::LoadFromBinaryFile(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }
}
