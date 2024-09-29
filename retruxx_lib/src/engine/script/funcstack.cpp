#include <script/funcstack.h>
#include <stdexcept>

namespace m3d
{
    sArgStack::~sArgStack()
    {
    }

    sArg* sArgStack::popIn()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    sArg* sArgStack::newOut()
    {
        throw std::logic_error("Not implemented");
    }

    sArg* sArgStack::newIn()
    {
        return &m_InArgs[m_numInArgs++];
    }
}
