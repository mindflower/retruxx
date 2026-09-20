#include <stdexcept>
#include <core/aiparam.h>
#include "core/ini.h"
#include <core/kernel.h>
#include <math/quaternion.h>
#include <math/vector.h>
#include <math/vector2.h>

namespace m3d
{
    int AIParam::CompareInt(void const* v1, void const* v2)
    {
        // RVA 0x613C40
        // NOTE: this subtracts the two *pointers*, not the ints they point at, so as a qsort
        // comparator it orders a contiguous array by address and leaves it exactly as it was.
        // Every list comparison below therefore compares unsorted lists element by element.
        return static_cast<int>(static_cast<char const*>(v1) - static_cast<char const*>(v2));
    }

    int AIParam::CompareStr(void const* v1, void const* v2)
    {
        // RVA 0x613C50 - an empty string sorts before everything else.
        char const* const a = *static_cast<char const* const*>(v1);
        char const* const b = *static_cast<char const* const*>(v2);

        bool const aEmpty = !a || !strlen(a);
        bool const bEmpty = !b || !strlen(b);

        if (aEmpty)
        {
            return bEmpty ? 0 : -1;
        }
        if (bEmpty)
        {
            return 1;
        }
        return strcmp(a, b);
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
        // RVA 0x615040 - a string is split into ids on the way out; anything that is not a
        // string or an id list reads back as an empty list.
        retruxx::vector<int> retVal;

        if (Type == AIPARAM_STRING)
        {
            retruxx::vector<CStr> tokens;
            Tokenize(*m_Str, tokens, "(), ;\t");
            for (auto const& token : tokens)
            {
                retVal.push_back(atoi(token.c_str()));
            }
        }
        else if (Type == AIPARAM_ID_LIST && m_NumList)
        {
            return *m_NumList;
        }

        return retVal;
    }

    CVector2 AIParam::GetAsRange() const
    {
        // RVA 0x615330
        // NOTE: a range keeps its second value in z, not y - see the CVector2 constructor.
        CVector const vec = GetAsVector();
        return CVector2(vec.x, vec.z);
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
            CStr res;
            for (unsigned i = 0; i < m_NumList->size(); ++i)
            {
                res += CStr((*m_NumList)[i]) + CStr(" ");
            }
            // Drop the separator the last entry left behind. An empty list leaves a length of
            // 0, so the end position works out to CStr_npos and the whole (empty) string is
            // returned instead.
            int const len = res.c_str() ? static_cast<int>(strlen(res.c_str())) : 0;
            return res.substr(0, len - 1);
        }
        case AIPARAM_STRING_LIST:
        {
            CStr res;
            for (unsigned i = 0; i < m_NameList->size(); ++i)
            {
                res += (*m_NameList)[i] + CStr(" ");
            }
            int const len = res.c_str() ? static_cast<int>(strlen(res.c_str())) : 0;
            return res.substr(0, len - 1);
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

    void AIParam::SaveToXML(cmn::XmlFile*, cmn::XmlNode* OwnNode) const
    {
        // RVA 0x6141F0 - the type always goes out; the value only when there is one.
        char const* typeName = "AIPARAM_UNDEFINE";
        switch (Type)
        {
        case AIPARAM_VECTOR: typeName = "AIPARAM_VECTOR"; break;
        case AIPARAM_QUATERNION: typeName = "AIPARAM_QUATERNION"; break;
        case AIPARAM_ID: typeName = "AIPARAM_ID"; break;
        case AIPARAM_FLOAT: typeName = "AIPARAM_FLOAT"; break;
        case AIPARAM_STRING: typeName = "AIPARAM_STRING"; break;
        case AIPARAM_ID_LIST: typeName = "AIPARAM_ID_LIST"; break;
        case AIPARAM_STRING_LIST: typeName = "AIPARAM_STRING_LIST"; break;
        case AIPARAM_RANGE: typeName = "AIPARAM_RANGE"; break;
        default: break;
        }

        OwnNode->SetAttribute("GAIParam_Type", typeName);
        if (Type != AIPARAM_UNDEFINE)
        {
            OwnNode->SetAttribute("GAIParam_Value", GetAsStr().c_str());
        }
    }

    retruxx::vector<CStr> AIParam::GetAsStringList() const
    {
        // RVA 0x615130 - the mirror of GetAsIdList.
        retruxx::vector<CStr> retVal;

        if (Type == AIPARAM_STRING)
        {
            Tokenize(*m_Str, retVal, "(), ;\t");
        }
        else if (Type == AIPARAM_STRING_LIST && m_NameList)
        {
            return *m_NameList;
        }

        return retVal;
    }

    void AIParam::Clear()
    {
        // RVA 0x946320
        // NOTE: the two name callbacks are deliberately left in place, unlike in Init.
        Detach();
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
        Type = AIPARAM_UNDEFINE;
    }

    void AIParam::ReadFromString(CStr const& Str)
    {
        // RVA 0x615C60 - guesses the type from how the string tokenises: one number is an id
        // or a float, two are a range, three a vector, and anything else a list.
        retruxx::vector<CStr> tokens;
        Tokenize(Str, tokens, "(), ;\t");

        auto const isNumeric = [](CStr const& token)
        {
            char const c = token.c_str()[0];
            return (c >= '0' && c <= '9') || c == '-';
        };

        switch (tokens.size())
        {
        case 0:
            Type = AIPARAM_UNDEFINE;
            return;

        case 1:
            if (isNumeric(tokens[0]))
            {
                // A decimal point is what separates a float from an id.
                if (tokens[0].find('.', 0) == -1)
                {
                    *this = atoi(tokens[0].c_str());
                }
                else
                {
                    *this = static_cast<float>(atof(tokens[0].c_str()));
                }
                return;
            }
            break;

        case 2:
            if (isNumeric(tokens[0]) && isNumeric(tokens[1]))
            {
                *this = CVector2(
                    static_cast<float>(atof(tokens[0].c_str())), static_cast<float>(atof(tokens[1].c_str())));
                return;
            }
            break;

        case 3:
            if (isNumeric(tokens[0]) && isNumeric(tokens[1]) && isNumeric(tokens[2]))
            {
                *this = CVector(static_cast<float>(atof(tokens[0].c_str())),
                    static_cast<float>(atof(tokens[1].c_str())), static_cast<float>(atof(tokens[2].c_str())));
                return;
            }
            break;

        default:
            break;
        }

        // Anything else is a list. It is held as a string first so that the list getters can
        // do the splitting, then replaced by the list they produce.
        *this = Str;
        if (isNumeric(tokens[0]))
        {
            *this = GetAsIdList();
        }
        else
        {
            *this = GetAsStringList();
        }
    }

    bool AIParam::operator==(AIParam const& with)
    {
        // RVA 0x6144B0 - like for like is compared by value; anything else falls back to
        // comparing the two printed forms.
        // Floats are compared with a tolerance rather than exactly.
        float const epsilon = 0.000099999997f;

        if (Type == with.Type)
        {
            switch (Type)
            {
            case AIPARAM_UNDEFINE:
                return true;

            case AIPARAM_VECTOR:
                return fabs(with.x - x) < epsilon && fabs(with.y - y) < epsilon
                    && fabs(with.z - z) < epsilon;

            case AIPARAM_QUATERNION:
                return fabs(with.x - x) < epsilon && fabs(with.y - y) < epsilon
                    && fabs(with.z - z) < epsilon && fabs(with.w - w) < epsilon;

            case AIPARAM_RANGE:
                // A range keeps its second value in z, so y is skipped.
                return fabs(with.x - x) < epsilon && fabs(with.z - z) < epsilon;

            case AIPARAM_ID:
                return id == with.id;

            case AIPARAM_FLOAT:
                return fabs(x - with.x) < epsilon;

            case AIPARAM_STRING:
                return *m_Str == *with.m_Str;

            case AIPARAM_ID_LIST:
            {
                // Order is not meant to matter, so both are sorted before comparing - though
                // see the note on CompareInt, which makes the sort a no-op.
                if (m_NumList->size() != with.m_NumList->size())
                {
                    return false;
                }

                std::vector<int> tmpV(*m_NumList);
                std::vector<int> tmpV2(*with.m_NumList);
                qsort(tmpV.data(), tmpV.size(), sizeof(int), CompareInt);
                qsort(tmpV2.data(), tmpV2.size(), sizeof(int), CompareInt);

                for (unsigned i = 0; i < tmpV.size(); ++i)
                {
                    if (tmpV[i] != tmpV2[i])
                    {
                        return false;
                    }
                }
                return true;
            }

            case AIPARAM_STRING_LIST:
            {
                if (m_NameList->size() != with.m_NameList->size())
                {
                    return false;
                }

                std::vector<CStr> tmpV2(*m_NameList);
                std::vector<CStr> tmpV(*with.m_NameList);
                qsort(tmpV2.data(), tmpV2.size(), sizeof(CStr), CompareStr);
                qsort(tmpV.data(), tmpV.size(), sizeof(CStr), CompareStr);

                for (unsigned i = 0; i < tmpV2.size(); ++i)
                {
                    if (tmpV2[i] != tmpV[i])
                    {
                        return false;
                    }
                }
                return true;
            }

            default:
                // A type outside the enum falls through to the string comparison below.
                break;
            }
        }

        // NOTE: an undefined value never equals anything by this route, not even another
        // undefined one - those only match through the AIPARAM_UNDEFINE case above, which
        // needs both types to be identical.
        if (Type != AIPARAM_UNDEFINE && with.Type != AIPARAM_UNDEFINE)
        {
            return with.GetAsStr() == GetAsStr();
        }
        return false;
    }

    bool AIParam::operator<=(AIParam const& with)
    {
        // RVA 0x946010
        return !(*this > with);
    }

    bool AIParam::operator!=(AIParam const& with)
    {
        // RVA 0x689F10
        return !(*this == with);
    }

    bool AIParam::operator<(AIParam const& with)
    {
        // RVA 0x6153F0 - the other side is read through its getters, so a string compares
        // against a number by being converted first.
        switch (Type)
        {
        case AIPARAM_UNDEFINE:
            // Anything defined outranks an undefined value.
            return with.Type != AIPARAM_UNDEFINE;

        case AIPARAM_VECTOR:
            // Vectors are ordered by length.
            return with.GetAsVector().length() > CVector(x, y, z).length();

        case AIPARAM_ID:
            return id < with.GetAsID();

        case AIPARAM_FLOAT:
            return with.GetAsFloat() > x;

        case AIPARAM_STRING:
            return *m_Str < with.GetAsStr();

        case AIPARAM_ID_LIST:
        {
            std::vector<int> List = with.GetAsIdList();
            if (m_NumList->size() != List.size())
            {
                // Different lengths are decided by length alone.
                return m_NumList->size() < List.size();
            }

            std::vector<int> tmpV(*m_NumList);
            qsort(tmpV.data(), tmpV.size(), sizeof(int), CompareInt);
            qsort(List.data(), List.size(), sizeof(int), CompareInt);

            // Only smaller when every element is smaller.
            for (unsigned i = 0; i < tmpV.size(); ++i)
            {
                if (tmpV[i] >= List[i])
                {
                    return false;
                }
            }
            return true;
        }

        case AIPARAM_STRING_LIST:
        {
            std::vector<CStr> List = with.GetAsStringList();

            // NOTE: this side's length is taken by dividing the list's byte span by 4 instead
            // of by sizeof(CStr), so it comes out three times too large. Two string lists are
            // therefore only compared element by element when this one holds exactly a third
            // as many entries as the other; any other pair is decided by the inflated length.
            unsigned const inflatedSize = m_NameList->size() * (sizeof(CStr) / 4);

            if (inflatedSize != List.size())
            {
                return inflatedSize < List.size();
            }

            std::vector<CStr> tmpV(*m_NameList);
            qsort(tmpV.data(), tmpV.size(), sizeof(CStr), CompareStr);
            qsort(List.data(), List.size(), sizeof(CStr), CompareStr);

            for (unsigned i = 0; i < tmpV.size(); ++i)
            {
                // CStr has no operator>=, and its operator< is a total order, so this is it.
                if (!(tmpV[i] < List[i]))
                {
                    return false;
                }
            }
            return true;
        }

        case AIPARAM_RANGE:
            // NOTE: the other side is still converted to a vector here, and the result thrown
            // away - ranges have no ordering and always compare false, in both directions.
            with.GetAsVector();
            return false;

        default:
            return false;
        }
    }

    AIParam& AIParam::operator=(CVector2 const& range)
    {
        // RVA 0x614C70
        Detach();
        Init();
        x = range.x;
        // NOTE: the second value goes into y here, but the CVector2 constructor puts it in z
        // and every reader - GetAsVector, GetAsRange, GetAsStr - looks in z. A range that was
        // assigned rather than constructed therefore reads back with a second value of 0.
        y = range.y;
        Type = AIPARAM_RANGE;
        return *this;
    }

    AIParam& AIParam::operator=(CVector const& PPos)
    {
        Detach();
        x = PPos.x;
        y = PPos.y;
        z = PPos.z;
        w = 0;
        Type = AIPARAM_VECTOR;
        return *this;
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

    AIParam& AIParam::operator=(retruxx::vector<CStr> const& list)
    {
        // RVA 0x614C00
        Detach();
        Init();
        Type = AIPARAM_STRING_LIST;
        m_NameList = new retruxx::vector<CStr>(list);
        return *this;
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

    AIParam& AIParam::operator=(retruxx::vector<int> const& list)
    {
        // RVA 0x614B90 - the list is copied onto the heap and owned from here on.
        Detach();
        Init();
        Type = AIPARAM_ID_LIST;
        m_NumList = new retruxx::vector<int>(list);
        return *this;
    }

    AIParam& AIParam::operator=(unsigned const& PID)
    {
        // RVA 0x5FE540
        Detach();
        Init();
        id = PID;
        Type = AIPARAM_ID;
        return *this;
    }

    AIParam& AIParam::operator=(Quaternion const& q)
    {
        // RVA 0x40BCF0
        Detach();
        Init();
        x = q.x;
        y = q.y;
        z = q.z;
        w = q.w;
        Type = AIPARAM_QUATERNION;
        return *this;
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

    bool AIParam::operator>(AIParam const& with)
    {
        // RVA 0x615780 - the mirror of operator< above, including its string-list length bug.
        switch (Type)
        {
        case AIPARAM_VECTOR:
            return CVector(x, y, z).length() > with.GetAsVector().length();

        case AIPARAM_ID:
            return id > with.GetAsID();

        case AIPARAM_FLOAT:
            return x > with.GetAsFloat();

        case AIPARAM_STRING:
            return *m_Str > with.GetAsStr();

        case AIPARAM_ID_LIST:
        {
            std::vector<int> List = with.GetAsIdList();
            if (m_NumList->size() != List.size())
            {
                return m_NumList->size() > List.size();
            }

            std::vector<int> tmpV(*m_NumList);
            qsort(tmpV.data(), tmpV.size(), sizeof(int), CompareInt);
            qsort(List.data(), List.size(), sizeof(int), CompareInt);

            for (unsigned i = 0; i < tmpV.size(); ++i)
            {
                if (tmpV[i] <= List[i])
                {
                    return false;
                }
            }
            return true;
        }

        case AIPARAM_STRING_LIST:
        {
            std::vector<CStr> List = with.GetAsStringList();

            // The same inflated length as in operator<.
            unsigned const inflatedSize = m_NameList->size() * (sizeof(CStr) / 4);

            if (inflatedSize != List.size())
            {
                return inflatedSize > List.size();
            }

            std::vector<CStr> tmpV(*m_NameList);
            qsort(tmpV.data(), tmpV.size(), sizeof(CStr), CompareStr);
            qsort(List.data(), List.size(), sizeof(CStr), CompareStr);

            for (unsigned i = 0; i < tmpV.size(); ++i)
            {
                // As above: no operator<= on CStr, so the negation of operator> stands in.
                if (!(tmpV[i] > List[i]))
                {
                    return false;
                }
            }
            return true;
        }

        case AIPARAM_RANGE:
            with.GetAsVector();
            return false;

        case AIPARAM_UNDEFINE:
        default:
            // NOTE: unlike operator<, there is no case for an undefined value here, so it is
            // never greater than anything - not even a defined value.
            return false;
        }
    }

    void AIParam::Init()
    {
        // RVA 0x4052D0
        id = 0;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
        NameFromNum = nullptr;
        NumFromName = nullptr;
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
        // RVA 0x614440
        return GetAsStr();
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
            // Components with no token keep these, so "1 2" comes back as (1, 2, 0).
            vec->x = 0.0f;
            vec->y = 0.0f;
            vec->z = 0.0f;
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
            vec->x = 0.0f;
            vec->y = 0.0f;
            vec->z = 0.0f;
            // An incomplete quaternion is left as the identity rather than all zeroes.
            vec->w = 1.0f;
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

        case AIPARAM_RANGE:
        {
            std::vector<CStr> tokens;
            m3d::Tokenize(m_Str, tokens, "(), ;\t");
            const auto size = tokens.size();
            auto range = (CVector2*)retVal;
            range->x = 0.0f;
            range->y = 0.0f;
            if (size > 0)
            {
                range->x = atof(tokens[0].c_str());
            }
            if (size > 1)
            {
                range->y = atof(tokens[1].c_str());
            }
            return;
        }

        case AIPARAM_ID:
        {
            *(int*)retVal = atoi(m_Str->c_str());
            return;
        }

        case AIPARAM_FLOAT:
        {
            *(float*)retVal = atof(m_Str->c_str());
            return;
        }

        case AIPARAM_STRING:
        {
            *(CStr*)retVal = *m_Str;
            return;
        }

        case AIPARAM_ID_LIST:
        {
            std::vector<CStr> tokens;
            m3d::Tokenize(m_Str, tokens, "(), ;\t");
            auto res = (std::vector<int>*)retVal;
            for (unsigned i = 0; i < tokens.size(); ++i)
            {
                res->push_back(atoi(tokens[i].c_str()));
            }
            return;
        }

        case AIPARAM_STRING_LIST:
        {
            std::vector<CStr> tokens;
            m3d::Tokenize(m_Str, tokens, "(), ;\t");
            auto res = (std::vector<CStr>*)retVal;
            for (unsigned i = 0; i < tokens.size(); ++i)
            {
                res->push_back(tokens[i]);
            }
            return;
        }

        default:
            return;
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
