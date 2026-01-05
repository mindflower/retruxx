#pragma once
#include <core/clazz.h>
#include <math/quaternion.h>

namespace m3d
{
    struct sArg
    {
        enum eArgType
        {
            ARGTYPE_VOID = 0,
            ARGTYPE_INT = 1,
            ARGTYPE_FLOAT = 2,
            ARGTYPE_BOOL = 3,
            ARGTYPE_STRING = 4,
            ARGTYPE_VECTOR = 5,
            ARGTYPE_OBJECT = 6,
            ARGTYPE_QUATERNION = 7,
        };

        sArg(m3d::sArg::eArgType, const void*);
        sArg(const m3d::sArg&);
        sArg();
        ~sArg();
        bool operator==(const m3d::sArg&) const;
        void SetI(int);
        void SetF(float f);
        void SetB(bool b);
        void SetS(const char* s);
        void SetO(m3d::Object* o);
        void SetV(const CVector& v);
        void SetQ(const Quaternion& v);
        m3d::sArg::eArgType GetType() const;
        int GetI() const;
        float GetF() const;
        bool GetB() const;
        const char* GetS() const;
        m3d::Object* GetO() const;
        CVector GetV() const;
        Quaternion GetQ() const;

        /* 0x0000 */ m3d::sArg::eArgType m_type = ARGTYPE_VOID;
        union
        {
            /* 0x0004 */ int m_i = 0;
            /* 0x0004 */ float m_f;
            /* 0x0004 */ bool m_b;
            /* 0x0004 */ char* m_s;
            /* 0x0004 */ m3d::Object* m_o;
            /* 0x0004 */ float m_v[3];
            /* 0x0004 */ float m_q[4];
        }; /* size: 0x000c */
    }; /* size: 0x0014 */

    static_assert(sizeof(sArg) == 0x0014);
}
