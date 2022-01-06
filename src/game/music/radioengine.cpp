#include "radioengine.h"
#include <stdexcept>

namespace m3d
{
    void RadioEngine::Init()
    {
        throw std::logic_error("Not implemented");
    }

    RadioEngine::~RadioEngine()
    {
        throw std::logic_error("Not implemented");
    }

    void RadioEngine::PlayNextSoundMessage()
    {
        throw std::logic_error("Not implemented");
    }

    RadioEngine* RadioEngine::GetInstance()
    {
        throw std::logic_error("Not implemented");
    }

    void RadioEngine::PlaySoundMessage(int, int, CStr const&)
    {
        throw std::logic_error("Not implemented");
    }
}
