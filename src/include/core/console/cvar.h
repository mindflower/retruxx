#pragma once
#include <core/stringm3d.h>

namespace m3d
{
    class IConHandler;

    class CVar
    {
    public:
        enum eType
        {
            CVAR_UNDEFINED = 0x0,
            CVAR_INT = 0x1,
            CVAR_FLOAT = 0x2,
            CVAR_BOOL = 0x3,
            CVAR_STRING = 0x4,
            CVAR_COLOR = 0x5,
        };

        enum eFlags
        {
            CVAR_ARCHIVE = 0x1,
            CVAR_READONLY = 0x2,
            CVAR_DEBUG = 0x4,
        };

    public:
        CVar();
        CVar(char const*, char const*, eType, eFlags);
        ~CVar();

        char const* GetDefault() const;
        char const* GetName() const;
        IConHandler* GetHandler();
        char const* GetS() const;
        float GetF() const;
        unsigned int GetC() const;
        bool GetB() const;
        int GetI() const;
        enum eFlags GetFlags() const;
        void Init(char const*, char const*, eType, eFlags);
        void SetHandler(IConHandler*);
        void Set(char const*, bool = false);
        void SetF(float, bool = false);
        void SetB(bool, bool = false);
        void SetI(int, bool = false);
        eType GetType() const;

    private:
        CStr m_name;
        eType m_type;
        eFlags m_flags;
        //$9B7D1A9EEEDF759822A9A331826DEE33 ___u3;
        CStr m_s;
        char* m_defaultValue;
        IConHandler* m_handler;
    };
}
