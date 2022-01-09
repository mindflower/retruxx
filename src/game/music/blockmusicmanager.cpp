#include "blockmusicmanager.h"
#include <stdexcept>

namespace m3d
{
    void BlockMusicManager::SetMusicType(BlockMusicType)
    {
        throw std::logic_error("Not implemented");
    }

    bool BlockMusicManager::IsMusicBlockNameValid(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    void BlockMusicManager::Reset()
    {
        throw std::logic_error("Not implemented");
    }

    void BlockMusicManager::Init()
    {
        throw std::logic_error("Not implemented");
    }

    void BlockMusicManager::InitOnce()
    {
        throw std::logic_error("Not implemented");
    }

    int BlockMusicManager::PlayCurrentMusic()
    {
        throw std::logic_error("Not implemented");
    }

    void BlockMusicManager::_MusicEndCallback(int)
    {
        throw std::logic_error("Not implemented");
    }
}
