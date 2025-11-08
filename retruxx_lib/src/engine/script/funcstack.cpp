#include <cassert>
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
        assert(m_curInArg < m_numInArgs);
        auto result = &this->m_InArgs[m_curInArg];
        this->m_curInArg = m_curInArg + 1;
        return result;
    }

    unsigned sArgStack::getNumOutArgs() const
    {
        return m_numOutArgs;
    }

    sArg* sArgStack::popOut()
    {
        assert(m_curOutArg < m_numOutArgs);
        return &this->m_OutArgs[m_curOutArg++];
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

    m3d::sArg* sArgStack::getOut(unsigned int i)
    {
        return &m_OutArgs[i];
    }

    sArg* sArgStack::newIn()
    {
        return &m_InArgs[m_numInArgs++];
    }
}
