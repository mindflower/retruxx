#include <cassert>
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
        assert(m_type == ARGTYPE_QUATERNION);

        return Quaternion(m_q[0], m_q[1], m_q[2], m_q[3]);
    }

    char const* sArg::GetS() const
    {
        return m_s;
    }

    float sArg::GetF() const
    {
        assert(m_type == ARGTYPE_FLOAT || m_type == ARGTYPE_INT);
        if (m_type == ARGTYPE_FLOAT)
            return m_f;
        else
            return static_cast<float>(m_i);
    }

    bool sArg::GetB() const
    {
        assert(m_type == ARGTYPE_BOOL || m_type == ARGTYPE_OBJECT || m_type == ARGTYPE_FLOAT || m_type == ARGTYPE_INT);
        
        if (m_type == ARGTYPE_BOOL)
            return this->m_b;

        if (m_type == ARGTYPE_FLOAT)
            return fabs(m_f) > 0.0000099999997;

        return m_i != 0;
    }

    Object* sArg::GetO() const
    {
        return m_o;
    }

    int sArg::GetI() const
    {
        assert(m_type == ARGTYPE_INT);
        return m_i;
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
        m_type = ARGTYPE_VECTOR;
        m_v[0] = v.x;
        m_v[1] = v.y;
        m_v[2] = v.z;
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
        m_q[0] = q.x;
        m_q[1] = q.y;
        m_q[2] = q.z;
        m_q[3] = q.w;
    }

    void sArg::SetF(float f)
    {
        m_type = ARGTYPE_FLOAT;
        m_f = f;
    }

    void sArg::SetB(bool b)
    {
        m_type = ARGTYPE_BOOL;
        m_b = b;
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
