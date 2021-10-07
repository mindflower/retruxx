#include <core/aiparam.h>
#include <core/kernel.h>
#include <math/quaternion.h>
#include <math/vector.h>
#include <math/vector2.h>

namespace m3d
{
    AIParam::AIParam(AIParam const& param)
    {
        if (this != &param)
        {
            Copy(param);
        }
    }

    AIParam::AIParam(CVector const& pos) :
        Type(AIPARAM_VECTOR),
        x(pos.x),
        y(pos.y),
        z(pos.z)
    {
    }
    //TODO: check z y
    AIParam::AIParam(CVector2 const& range) :
        Type(AIPARAM_RANGE),
        x(range.x),
        z(range.y)
    {
    }

    AIParam::AIParam(std::vector<int> const& list) :
        Type(AIPARAM_ID_LIST)
    {
        m_NumList = new std::vector<int>(list);
    }

    AIParam::AIParam(CStr const& str) :
        Type(AIPARAM_STRING)
    {
        m_Str = new CStr(str);
    }

    AIParam::AIParam(int const& pid) :
        Type(AIPARAM_ID),
        id(pid)
    {
    }

    AIParam::AIParam(float const& value) :
        Type(AIPARAM_FLOAT),
        Value(value)
    {
    }

    AIParam::AIParam(Quaternion const& pos) :
        Type(AIPARAM_QUATERNION),
        x(pos.x),
        y(pos.y),
        z(pos.z),
        w(pos.w)
    {

    }

    AIParam::~AIParam()
    {
        Detach();
    }

    int AIParam::GetAsID() const
    {
        if (GetType() == AIPARAM_UNDEFINE)
        {
            return -1;
        }
        if (GetType() == AIPARAM_FLOAT)
        {
            return x;
        }
        if (GetType() != AIPARAM_VECTOR)
        {
            return id;
        }
        if (m_Str && !m_Str->empty())
        {
            return stoi(*m_Str);
        }
        return 0;
    }

    eAIParamType AIParam::GetType() const
    {
        return Type;
    }

    void AIParam::Copy(AIParam const& param)
    {
        //TODO: refactor this shit
        if (&param != this)
        {
            id = 0;
            switch (Type)
            {
            case AIPARAM_STRING:
            {
                if (m_Str == param.m_Str)
                {
                    break;
                }
                M3D_ASSERT(!m_Str);
                if (!param.m_Str)
                {
                    break;
                }
                m_Str = new CStr(*param.m_Str);
                break;
            }
            case AIPARAM_ID_LIST:
            {
                if (m_NumList == param.m_NumList)
                {
                    break;
                }
                M3D_ASSERT(!m_NumList);
                if (!param.m_NumList)
                {
                    break;
                }
                m_NumList = new std::vector(*param.m_NumList);
                break;
            }
            case AIPARAM_STRING_LIST:
            {
                if (m_NameList == param.m_NameList)
                {
                    break;
                }
                M3D_ASSERT(!m_NameList);
                if (!param.m_NameList)
                {
                    break;
                }
                m_NameList = new std::vector(*param.m_NameList);
                break;
            }
            default:
            {
                memcpy(this, &param, sizeof(AIParam));
                return;
            }
            }
            //NameFromNum = param.NameFromNum;
            //NumFromName = param.NumFromName;
            Type = param.Type;
        }
    }

    void AIParam::Detach()
    {
        switch (Type)
        {
        case AIPARAM_STRING:
        {
            if (m_Str)
            {
                delete m_Str;
            }
            break;
        }
        case AIPARAM_ID_LIST:
        {
            if (m_NumList)
            {
                delete m_NumList;
            }
            break;
        }
        case AIPARAM_STRING_LIST:
        {
            if (m_NameList)
            {
                delete m_NameList;
            }
            break;
        }
        }
        id = 0;
        Type = AIPARAM_UNDEFINE;
        //NameFromNum = 0;
        //NumFromName = 0;
        y = 0.0;
        z = 0.0;
    }
}
