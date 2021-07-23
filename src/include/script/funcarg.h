#pragma once
#include <core/clazz.h>
#include <math/quaternion.h>

namespace m3d
{
    class sArg
    {
    public:
        enum eArgType
        {
            ARGTYPE_VOID = 0x0,
            ARGTYPE_INT = 0x1,
            ARGTYPE_FLOAT = 0x2,
            ARGTYPE_BOOL = 0x3,
            ARGTYPE_STRING = 0x4,
            ARGTYPE_VECTOR = 0x5,
            ARGTYPE_OBJECT = 0x6,
            ARGTYPE_QUATERNION = 0x7,
        };

    public:
        ~sArg();
        Quaternion GetQ() const;
        char const* GetS() const;
        float GetF() const;
        bool GetB() const;
        Object* GetO() const;
        int GetI() const;
        sArg();
        CVector GetV() const;
        void SetV(struct CVector const&);
        void SetS(char const*);
        void SetQ(struct Quaternion const&);
        void SetF(float);
        void SetB(bool);
        void SetO(Object*);
        eArgType GetType() const;

    private:
        eArgType m_type;
        //$E386D9AD189E158338F05843C2D08083 ___u1;
    };
}
