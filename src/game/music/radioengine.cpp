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
        if (m_instance == this)
        {
            m_instance = 0;
        }
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
