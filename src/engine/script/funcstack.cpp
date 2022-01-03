#include <script/funcstack.h>
#include <stdexcept>

namespace m3d
{
    sArgStack::~sArgStack()
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }
}
