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
        CVar() = default;
        CVar(char const* name, char const* value, eType type, eFlags flags);

        void Init(char const* name, char const* value, eType type, eFlags flags);
        void SetB(bool b, bool ignoreFlags = true);
        void SetF(float f, bool ignoreFlags = true);
        void SetI(int i, bool ignoreFlags = true);
        void Set(char const* value, bool ignoreFlags = true);
        void SetHandler(IConHandler* handler);

        bool GetB() const;
        unsigned int GetC() const;
        float GetF() const;
        int GetI() const;
        char const* GetDefault() const;
        IConHandler* GetHandler() const;
        char const* GetName() const;
        char const* GetS() const;
        eType GetType() const;
        eFlags GetFlags() const;

    private:
        CStr m_name;
        eType m_type = CVAR_UNDEFINED;
        eFlags m_flags = CVAR_ARCHIVE;
        CStr m_s;
        CStr m_defaultValue;
        IConHandler* m_handler = nullptr;
        //TODO: union initialization 
        union
        {
            int m_i = 0;
            unsigned int m_color;
            float m_f;
            bool m_b;
        };
    };
}
