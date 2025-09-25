#include <script/funcstack.h>
#include <stdexcept>

#include "core/kernel.h"

namespace m3d
{
    sArgStack::~sArgStack()
    {
    }

    sArg* sArgStack::popIn()
    {
        M3D_ASSERT(m_curInArg < m_numInArgs);
        auto result = &this->m_InArgs[m_curInArg];
        this->m_curInArg = m_curInArg + 1;
        return result;
    }

    unsigned sArgStack::getNumOutArgs() const
    {
        throw std::logic_error("Not implemented");
    }

    sArg* sArgStack::popOut()
    {
        throw std::logic_error("Not implemented");
    }

    sArgStack::sArgStack()
    {
    }

    unsigned sArgStack::getNumInArgs() const
    {
        return this->m_numInArgs;
    }

    sArg* sArgStack::newOut()
    {
        return &m_OutArgs[m_numOutArgs++];
    }

    sArg* sArgStack::newIn()
    {
        return &m_InArgs[m_numInArgs++];
    }
}
