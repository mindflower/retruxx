#pragma once
#include <core/stringm3d.h>

#include <cstdint>

namespace m3d
{
    class IConHandler;

    //IMPORTANT: fields and members order is strict
    class CVar
    {
    public:
        enum eType : int32_t
        {
            CVAR_UNDEFINED = 0x0,
            CVAR_INT = 0x1,
            CVAR_FLOAT = 0x2,
            CVAR_BOOL = 0x3,
            CVAR_STRING = 0x4,
            CVAR_COLOR = 0x5,
        };

        enum eFlags : int32_t
        {
            CVAR_ARCHIVE = 0x1,
            CVAR_READONLY = 0x2,
            CVAR_DEBUG = 0x4,
        };

    public:
        CVar(CVar const&);
        CVar(char const* name, char const* value, eType type, eFlags flags);
        CVar();
        ~CVar();

        void Init(char const* name, char const* value, eType type, eFlags flags);
        int GetI() const;
        float GetF() const;
        bool GetB() const;
        char const* GetS() const;
        unsigned int GetC() const;


        void Set(char const* value, bool ignoreFlags = true);
        void SetI(int, bool ignoreFlags = true);
        void SetF(float f, bool ignoreFlags = true);
        void SetB(bool b, bool ignoreFlags = true);
        void SetC(unsigned int, bool ignoreFlags = true);

        eType GetType() const;
        eFlags GetFlags() const;
        char const* GetDefault() const;
        void ResetToDefault();

        void SetHandler(IConHandler* handler);
        IConHandler* GetHandler() const;
        char const* GetName() const;

    private:
        CStr m_name;
        eType m_type = CVAR_UNDEFINED;
        eFlags m_flags = CVAR_ARCHIVE;
        union
        {
            int32_t m_i = 0;
            uint32_t m_color;
            float m_f;
            bool m_b;
        };
        CStr m_s;
        char* m_defaultValue = nullptr;
        IConHandler* m_handler = nullptr;

        bool operator==(CVar const& rhs) const;
    };

    // The original driver DLLs read cvars straight out of EngineConfig: the type at +0x0c and
    // the value at +0x14, so the layout is fixed.
    static_assert(sizeof(CVar) == 0x002c);
}
