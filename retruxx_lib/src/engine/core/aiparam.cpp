#include <stdexcept>
#include <core/aiparam.h>
#include "core/ini.h"
#include <core/kernel.h>
#include <math/quaternion.h>
#include <math/vector.h>
#include <math/vector2.h>

namespace m3d
{
    int AIParam::CompareInt(void const*, void const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AIParam::CompareStr(void const*, void const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    AIParam::AIParam(retruxx::vector<int> const& list) :
        Type(AIPARAM_ID_LIST)
    {
        m_NumList = new retruxx::vector<int>(list);
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

    AIParam::AIParam()
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
        if (GetType() != AIPARAM_STRING)
        {
            return id;
        }
        if (m_Str && !m_Str->empty())
        {
            return atoi(m_Str->c_str());
        }
        return 0;
    }

    retruxx::vector<int> AIParam::GetAsIdList() const
    {
        if (Type == AIPARAM_ID_LIST)
        {
            return *m_NumList;
        }
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector2 AIParam::GetAsRange() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr AIParam::GetAsStr() const
    {
        switch(Type)
        {
        case AIPARAM_UNDEFINE:
        {
            return {};
	    }
        case AIPARAM_VECTOR:
        {
            CStr res;
            res.format("%.3f %.3f %.3f", this->x, this->y, this->z);
            return res;
        }
        case AIPARAM_QUATERNION:
        {
            CStr res;
            res.format("%.3f %.3f %.3f %.3f", this->x, this->y, this->z, this->w);
            return res;
        }
        case AIPARAM_ID:
        {
            CStr res;
            res.format("%ld", this->id);
            return res;
        }
        case AIPARAM_FLOAT:
        {
            CStr res;
            res.format("%.3f", this->x);
            return res;
        }
        case AIPARAM_STRING:
        {
            return *m_Str;
        }
        case AIPARAM_ID_LIST:
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
        case AIPARAM_STRING_LIST:
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
        case AIPARAM_RANGE:
        {
            CStr res;
            res.format("%.3f %.3f ", this->x, this->z);
            return res;
        }
        default:
        	return {};
        }
    }

    CVector AIParam::GetAsVector() const
    {
        CVector res;
        switch (Type)
        {
        case AIPARAM_VECTOR:
            res.x = x;
            res.y = y;
            res.z = z;
            return res;

        case AIPARAM_ID:
            res.x = id;
            return res;

        case AIPARAM_FLOAT:
            res.x = x;
            return res;

        case AIPARAM_STRING:
            ConvertFromString(&res, AIPARAM_VECTOR);
            return res;

        case AIPARAM_RANGE:
            res.x = x;
            res.z = z;
            res.y = 0.0;
            return res;

        default:
            return res;
        }
    }

    void AIParam::SaveToXML(cmn::XmlFile*, cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::vector<CStr> AIParam::GetAsStringList() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AIParam::Clear()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AIParam::ReadFromString(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool AIParam::operator==(AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool AIParam::operator<=(AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool AIParam::operator!=(AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool AIParam::operator<(AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    AIParam& AIParam::operator=(CVector2 const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    AIParam& AIParam::operator=(CVector const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    AIParam& AIParam::operator=(CStr const& str)
    {
        Detach();
        id = 0;
        w = 0;
        y = 0;
        m_Str = new CStr(str);
        Type = AIPARAM_STRING;
        return *this;
    }

    AIParam& AIParam::operator=(AIParam const& pparam)
    {
        if (&pparam == this)
        {
            return *this;
        }
        Detach();
        Copy(pparam);
        return *this;
    }

    AIParam& AIParam::operator=(retruxx::vector<CStr> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    AIParam& AIParam::operator=(float const& f)
    {
        Detach();
        z = 0;
        y = 0;
        w = 0;
        x = f;
        Type = AIPARAM_FLOAT;
        return *this;
    }

    AIParam& AIParam::operator=(retruxx::vector<int> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    AIParam& AIParam::operator=(unsigned const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    AIParam& AIParam::operator=(Quaternion const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    AIParam& AIParam::operator=(int const& i)
    {
        Detach();
        y = 0;
        z = 0;
        w = 0;
        Type = AIPARAM_ID;
        id = i;
        return *this;
    }

    bool AIParam::operator>(AIParam const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AIParam::Init()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AIParam::LoadFromXML(cmn::XmlFile* xmlFile, cmn::XmlNode const* xmlNode)
    {
        //TODO: check this
        Detach();
        CStr valueAttr = xmlNode->GetAttribute("GAIParam_Value");
        operator=(valueAttr);
        CStr type = xmlNode->GetAttribute("GAIParam_Type");
        if (type == "AIPARAM_UNDEFINE")
        {
            SetType(AIPARAM_UNDEFINE);
        }
        else if (type == "AIPARAM_VECTOR")
        {
            SetType(AIPARAM_VECTOR);
        }
        else if (type == "AIPARAM_QUATERNION")
        {
            SetType(AIPARAM_QUATERNION);
        }
        else if (type == "AIPARAM_ID")
        {
            SetType(AIPARAM_ID);
        }
        else if (type == "AIPARAM_FLOAT")
        {
            SetType(AIPARAM_FLOAT);
        }
        else if (type == "AIPARAM_STRING")
        {
            SetType(AIPARAM_STRING);
        }
        else if (type == "AIPARAM_ID_LIST")
        {
            SetType(AIPARAM_ID_LIST);
        }
        else if (type == "AIPARAM_STRING_LIST")
        {
            SetType(AIPARAM_STRING_LIST);
        }
        else if (type == "AIPARAM_RANGE")
        {
            SetType(AIPARAM_RANGE);
        }
    }

    float AIParam::GetAsFloat() const
    {
        double result; // st7
        float v2; // xmm1_4
        float* v3; // ebx
        float* v4; // edi
        float* v5; // esi
        const char** v6; // ecx
        float v7; // [esp+4h] [ebp-8h] BYREF
        float v8; // [esp+8h] [ebp-4h] BYREF

        switch (Type)
        {
        case AIPARAM_ID:
            return id;
        case AIPARAM_STRING:
            if (m_Str && !m_Str->empty())
                return atof(m_Str->c_str());
            return 0;
        case AIPARAM_RANGE:
            //TODO: check this!!!
            v2 = this->y;
            v7 = this->id;
            v8 = v2;
            v3 = &v8;
            if (*&v7 <= v2)
                v3 = &v7;
            v4 = &v8;
            if (v2 <= *&v7)
                v4 = &v7;
            v5 = &v8;
            if (*&v7 <= v2)
                v5 = &v7;
            return rand() * (*v4 - *v5) * 0.000030518509 + *v3;
        default:
            return x;
        }
    }

    Quaternion AIParam::GetAsQuaternion() const
    {
        Quaternion res;
        res.Identity();

        switch (Type)
        {
        case AIPARAM_STRING:
            ConvertFromString(&res, AIPARAM_QUATERNION);
            return res;

        case AIPARAM_QUATERNION:
            res.x = x;
            res.y = y;
            res.z = z;
            res.w = w;
            return res;

        default:
            return res;
        }
    }

    CStr AIParam::ToStr() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AIParam::SetType(eAIParamType ParamType)
    {
        switch (ParamType)
        {
        case AIPARAM_UNDEFINE:
            Detach();
            Type = AIPARAM_UNDEFINE;
            break;
        case AIPARAM_VECTOR:
            operator=(GetAsVector());
            break;
        case AIPARAM_QUATERNION:
            operator=(GetAsQuaternion());
            break;
        case AIPARAM_ID:
            operator=(GetAsID());
            break;
        case AIPARAM_FLOAT:
            operator=(GetAsFloat());
            break;
        case AIPARAM_STRING:
            operator=(GetAsStr());
            break;
        case AIPARAM_ID_LIST:
            operator=(GetAsIdList());
            break;
        case AIPARAM_STRING_LIST:
            operator=(GetAsStringList());
            break;
        case AIPARAM_RANGE:
            operator=(GetAsVector());
            break;
        default:
            return;
        }
    }

    void AIParam::ConvertFromString(void* retVal, eAIParamType ToType) const
    {
        switch (ToType)
        {
        case AIPARAM_VECTOR:
        {
            std::vector<CStr> tokens;
            m3d::Tokenize(m_Str, tokens, "(), ;\t");
            const auto size = tokens.size();
            auto vec = (CVector*)retVal;
            if (size > 0)
            {
                vec->x = atof(tokens[0].c_str());
            }
            if (size > 1)
            {
                vec->y = atof(tokens[1].c_str());
            }
            if (size > 2)
            {
                vec->z = atof(tokens[2].c_str());
            }
            return;
        }

        case AIPARAM_QUATERNION:
        {
            std::vector<CStr> tokens;
            m3d::Tokenize(m_Str, tokens, "(), ;\t");
            const auto size = tokens.size();
            auto vec = (Quaternion*)retVal;
            if (size > 0)
            {
                vec->x = atof(tokens[0].c_str());
            }
            if (size > 1)
            {
                vec->y = atof(tokens[1].c_str());
            }
            if (size > 2)
            {
                vec->z = atof(tokens[2].c_str());
            }
            if (size > 3)
            {
                vec->w = atof(tokens[3].c_str());
            }
            return;
        }

        default:
            RETRUXX_NOT_IMPLEMENTED;
        }
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
            switch (param.GetType())
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
                m_NumList = new retruxx::vector<int>(*param.m_NumList);
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
                m_NameList = new retruxx::vector<CStr>(*param.m_NameList);
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
