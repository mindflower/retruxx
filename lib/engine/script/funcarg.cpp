#include <cassert>
#include <script/funcarg.h>
#include <math/vector.h>
#include <stdexcept>
#include <Windows.h>

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
        assert(m_type == ARGTYPE_VECTOR);

        return Quaternion(m_q[0], m_q[1], m_q[2], m_q[3]);
    }

    char const* sArg::GetS() const
    {
        return m_s;
    }

    float sArg::GetF() const
    {
        assert(m_type == ARGTYPE_FLOAT || m_type == ARGTYPE_INT);
        if (this->m_type == ARGTYPE_FLOAT)
            return this->m_f;
        else
            return (float)this->m_i;
    }

    bool sArg::GetB() const
    {
        assert(m_type == ARGTYPE_BOOL || m_type == ARGTYPE_OBJECT || m_type == ARGTYPE_FLOAT || m_type == ARGTYPE_INT);
        
        if (this->m_type == ARGTYPE_BOOL)
            return this->m_b;

        if (this->m_type == ARGTYPE_FLOAT)
            return fabs(this->m_f) > 0.0000099999997;

        return this->m_i != 0;
    }

    Object* sArg::GetO() const
    {
        return m_o;
    }

    int sArg::GetI() const
    {
        assert(m_type == ARGTYPE_INT);
        return this->m_i;
    }

    sArg::sArg()
    {
    }

    CVector sArg::GetV() const
    {
        assert(m_type == ARGTYPE_VECTOR);

        return CVector(m_v[0], m_v[1], m_v[2]);
    }

    void sArg::SetV(CVector const& v)
    {
        this->m_type = ARGTYPE_VECTOR;
        *(CVector*)&this->m_i = v;
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

    void sArg::SetQ(Quaternion const& q)
    {
        this->m_type = ARGTYPE_QUATERNION;
        *(Quaternion*)&this->m_i = q;
    }

    void sArg::SetF(float f)
    {
        this->m_type = ARGTYPE_FLOAT;
        this->m_f = f;
    }

    void sArg::SetB(bool b)
    {
        this->m_type = ARGTYPE_BOOL;
        this->m_b = b;
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
