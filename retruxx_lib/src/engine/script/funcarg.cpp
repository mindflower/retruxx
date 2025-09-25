#include <script/funcarg.h>
#include <math/vector.h>
#include <stdexcept>

namespace m3d
{
    sArg::~sArg()
    {
        if (m_type == ARGTYPE_STRING)
        {
            delete[] m_s;
        }
    }

    Quaternion sArg::GetQ() const
    {
        throw std::logic_error("Not implemented");
    }

    char const* sArg::GetS() const
    {
        return  m_s;
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
    }

    CVector sArg::GetV() const
    {
        throw std::logic_error("Not implemented");
    }

    void sArg::SetV(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void sArg::SetS(char const* s)
    {
        if (m_type == ARGTYPE_STRING)
        {
            delete[] m_s;
        }
        m_s = nullptr;
        m_type = ARGTYPE_STRING;
        if (s)
        {
            auto len = strlen(s);
            m_s = new char[len + 1];
            strcpy(m_s, s);
            m_s[len] = '\0';
        }
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

    void sArg::SetO(Object* o)
    {
        m_type = ARGTYPE_OBJECT;
        m_o = o;
    }

    sArg::eArgType sArg::GetType() const
    {
        return m_type;
    }
}
