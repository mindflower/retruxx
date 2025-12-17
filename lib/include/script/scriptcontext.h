#pragma once
#include "core/clazz.h"

extern "C"
{
#include "lua.h"
}

namespace m3d
{
    class LuaContext : public Context
    {
    public:
        virtual void pushObject(Object*);
        virtual void pushAIParam(AIParam const&);
        virtual int asInt(int);
        virtual bool asBool(int);
        virtual Object* asObject(int, char const*);
        virtual int countArgs();
        virtual float asFloat(int);
        virtual AIParam& asAIParam(int);
        virtual void pushBool(bool);
        virtual char const* asString(int);
        virtual void pushQuaternion(Quaternion const&);
        virtual void pushInt(int x);
        virtual void pushFloat(float x);
        virtual Quaternion& asQuaternion(int);
        virtual void pushString(char const* x);
        virtual CVector& asVector(int i);
        virtual void pushVector(CVector const&);

        inline int _validateArg(int i)
        {
            if (i < 0)
            {
                lua_pushstring(this->L, "not enough arguments");
                lua_error(this->L);
            }
            return i + this->m_stackStart;
        }

    public:
        lua_State* L;
        int m_stackStart;
        int m_numInputs;
        int m_numOutputs;
    };
}
