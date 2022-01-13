#include <core/console/cvar.h>
#include <cstdio>
#include <stdexcept>

namespace
{
    //TODO: add tests
    unsigned int strToColor(CStr const& str, unsigned def)
    {
        if (!str.empty())
        {
            int colorArr[4] = { 0 };
            if (sscanf_s(str.c_str(), "%d %d %d %d", &colorArr[0], &colorArr[1], &colorArr[2], &colorArr[3]) == 4)
            {
                for (auto& elem : colorArr)
                {
                    if (elem > 255)
                    {
                        elem = 255;
                    }
                    if (elem < 0)
                    {
                        elem = 0;
                    }
                }
                unsigned int result = colorArr[2] | ((colorArr[1] | ((colorArr[0] | (colorArr[3] << 8)) << 8)) << 8);
                return result;
            }
        }
        return def;
    }
}

namespace m3d
{
    CVar::CVar(CVar const&)
    {
        throw std::logic_error("Not implemented");
    }

    CVar::CVar()
    {
    }

    CVar::CVar(char const* name, char const* value, eType type, eFlags flags)
    {
        Init(name, value, type, flags);
    }

    CVar::~CVar()
    {
        delete[] m_defaultValue;
    }

    void CVar::Init(char const* name, char const* value, eType type, eFlags flags)
    {
        m_name = name;
        m_type = type;
        m_flags = flags;
        Set(value);
    }

    void CVar::SetB(bool b, bool ignoreFlags)
    {
        char buffer[1] = { 0 };
        sprintf_s(buffer, "%d", b);
        Set(buffer, ignoreFlags);
    }

    void CVar::SetC(unsigned int i, bool ignoreFlags)
    {
        char buffer[32] = { 0 };
        sprintf_s(buffer, "%d", i);
        Set(buffer, ignoreFlags);
    }

    void CVar::SetF(float f, bool ignoreFlags)
    {
        char buffer[16] = { 0 };
        sprintf_s(buffer, "%.2f", f);
        Set(buffer, ignoreFlags);
    }

    void CVar::SetI(int i, bool ignoreFlags)
    {
        char buffer[16] = { 0 };
        sprintf_s(buffer, "%d", i);
        Set(buffer, ignoreFlags);
    }

    void CVar::Set(char const* value, bool ignoreFlags)
    {
        if (ignoreFlags || m_flags & CVAR_READONLY == 0)
        {
            switch (m_type)
            {
            case CVAR_INT:
            {
                if (sscanf_s(value, "%d", &m_i))
                {
                    m_s = value;
                }
                else
                {
                    m_i = 0;
                    m_s = "\" \"";
                }
                break;
            }
            case CVAR_FLOAT:
            {
                if (sscanf_s(value, "%f", &m_f))
                {
                    m_s = value;
                }
                else
                {
                    m_f = 0.0;
                    m_s = "\" \"";
                }
                break;
            }
            case CVAR_BOOL:
            {
                if (sscanf_s(value, "%d", &m_i))
                {
                    m_b = m_i > 0;
                    if (m_b)
                    {
                        m_s = "true";
                    }
                    else
                    {
                        m_s = "false";
                    }
                }
                else
                {
                    m_b =
                        !stricmp(value, "yes") ||
                        !stricmp(value, "yeah") ||
                        !stricmp(value, "yep") ||
                        !stricmp(value, "true");
                    if (m_b)
                    {
                        m_s = "true";
                    }
                    else
                    {
                        m_s = "false";
                    }
                }
                break;
            }
            case CVAR_COLOR:
            {
                m_color = strToColor(value, 0xFFFFFFFF);
                break;
            }
            case CVAR_STRING:
            {
                m_type = CVAR_STRING;
                m_s = value;
                break;
            }
            default:
            {
                m_type = CVAR_STRING;
                m_s = value;
                break;
            }
            }
        }
        if (!m_defaultValue)
        {
            m_s = value;
            m_defaultValue = new char[m_s.length() + 1];
            strcpy(m_defaultValue, m_s.c_str());
        }
    }

    void CVar::SetHandler(IConHandler* handler)
    {
        m_handler = handler;
    }

    bool CVar::GetB() const
    {
        return m_type == CVAR_BOOL ? m_b : m_i > 0;
    }

    unsigned CVar::GetC() const
    {
        return m_color;
    }

    float CVar::GetF() const
    {
        return m_f;
    }

    int CVar::GetI() const
    {
        return m_i;
    }

    char const* CVar::GetDefault() const
    {
        return m_defaultValue;
    }

    void CVar::ResetToDefault()
    {
        throw std::logic_error("Not implemented");
    }

    IConHandler* CVar::GetHandler() const
    {
        return m_handler;
    }

    char const* CVar::GetName() const
    {
        return m_name.c_str();
    }

    bool CVar::operator==(CVar const& rhs) const
    {
        throw std::logic_error("Not implemented");
    }

    CVar::eFlags CVar::GetFlags() const
    {
        return m_flags;
    }

    char const* CVar::GetS() const
    {
        return m_s.c_str();
    }

    CVar::eType CVar::GetType() const
    {
        return m_type;
    }
}
