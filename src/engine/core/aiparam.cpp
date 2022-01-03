#include <stdexcept>
#include <core/aiparam.h>
#include <core/kernel.h>
#include <math/quaternion.h>
#include <math/vector.h>
#include <math/vector2.h>

namespace m3d
{
    int AIParam::CompareInt(void const*, void const*)
    {
        throw std::logic_error("Not implemented");
    }

    int AIParam::CompareStr(void const*, void const*)
    {
        throw std::logic_error("Not implemented");
    }

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

    std::vector<int> AIParam::GetAsIdList() const
    {
        throw std::logic_error("Not implemented");
    }

    CVector2 AIParam::GetAsRange() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr AIParam::GetAsStr() const
    {
        throw std::logic_error("Not implemented");
    }

    CVector AIParam::GetAsVector() const
    {
        throw std::logic_error("Not implemented");
    }

    void AIParam::SaveToXML(cmn::XmlFile*, cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<CStr> AIParam::GetAsStringList() const
    {
        throw std::logic_error("Not implemented");
    }

    void AIParam::Clear()
    {
        throw std::logic_error("Not implemented");
    }

    void AIParam::ReadFromString(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool AIParam::operator==(AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool AIParam::operator<=(AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool AIParam::operator!=(AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool AIParam::operator<(AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    AIParam& AIParam::operator=(CVector2 const&)
    {
        throw std::logic_error("Not implemented");
    }

    AIParam& AIParam::operator=(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    AIParam& AIParam::operator=(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    AIParam& AIParam::operator=(AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    AIParam& AIParam::operator=(std::vector<CStr> const&)
    {
        throw std::logic_error("Not implemented");
    }

    AIParam& AIParam::operator=(float const&)
    {
        throw std::logic_error("Not implemented");
    }

    AIParam& AIParam::operator=(std::vector<int> const&)
    {
        throw std::logic_error("Not implemented");
    }

    AIParam& AIParam::operator=(unsigned const&)
    {
        throw std::logic_error("Not implemented");
    }

    AIParam& AIParam::operator=(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
    }

    AIParam& AIParam::operator=(int const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool AIParam::operator>(AIParam const&)
    {
        throw std::logic_error("Not implemented");
    }

    void AIParam::Init()
    {
        throw std::logic_error("Not implemented");
    }

    void AIParam::LoadFromXML(cmn::XmlFile*, cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    float AIParam::GetAsFloat() const
    {
        throw std::logic_error("Not implemented");
    }

    Quaternion AIParam::GetAsQuaternion() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr AIParam::ToStr() const
    {
        throw std::logic_error("Not implemented");
    }

    void AIParam::SetType(eAIParamType)
    {
        throw std::logic_error("Not implemented");
    }

    void AIParam::ConvertFromString(void*, eAIParamType) const
    {
        throw std::logic_error("Not implemented");
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
