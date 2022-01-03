#include <script/funcarg.h>
#include <math/vector.h>
#include <stdexcept>

namespace m3d
{
    sArg::~sArg()
    {
        throw std::logic_error("Not implemented");
    }

    Quaternion sArg::GetQ() const
    {
        throw std::logic_error("Not implemented");
    }

    char const* sArg::GetS() const
    {
        throw std::logic_error("Not implemented");
    }

    float sArg::GetF() const
    {
        throw std::logic_error("Not implemented");
    }

    bool sArg::GetB() const
    {
        throw std::logic_error("Not implemented");
    }

    Object* sArg::GetO() const
    {
        throw std::logic_error("Not implemented");
    }

    int sArg::GetI() const
    {
        throw std::logic_error("Not implemented");
    }

    sArg::sArg()
    {
        throw std::logic_error("Not implemented");
    }

    CVector sArg::GetV() const
    {
        throw std::logic_error("Not implemented");
    }

    void sArg::SetV(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void sArg::SetS(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void sArg::SetQ(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
    }

    void sArg::SetF(float)
    {
        throw std::logic_error("Not implemented");
    }

    void sArg::SetB(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void sArg::SetO(Object*)
    {
        throw std::logic_error("Not implemented");
    }

    sArg::eArgType sArg::GetType() const
    {
        throw std::logic_error("Not implemented");
    }
}
