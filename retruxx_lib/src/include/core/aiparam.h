#pragma once
#include "stringm3d.h"
#include "thirdparty/stl/vector.hpp"

class CVector;
class CVector2;
class Quaternion;

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }

    enum eAIParamType
    {
        AIPARAM_UNDEFINE = 0x0,
        AIPARAM_VECTOR = 0x1,
        AIPARAM_QUATERNION = 0x2,
        AIPARAM_ID = 0x3,
        AIPARAM_FLOAT = 0x4,
        AIPARAM_STRING = 0x5,
        AIPARAM_ID_LIST = 0x6,
        AIPARAM_STRING_LIST = 0x7,
        AIPARAM_RANGE = 0x8,
    };

    class AIParam
    {
    protected:
        union
        {
            struct
            {
                /* 0x0000 */ float x;
                /* 0x0004 */ float y;
                /* 0x0008 */ float z;
                /* 0x000c */ float w;
            }; /* size: 0x0010 */
            /* 0x0000 */ int id;
            /* 0x0000 */ float Value;
            /* 0x0000 */ oldstd::vector<CStr>* m_NameList;
            /* 0x0000 */ oldstd::vector<int>* m_NumList;
            /* 0x0000 */ CStr* m_Str;
        }; /* size: 0x0010 */
        /* 0x0010 */ m3d::eAIParamType Type;
        void Detach();
        void Copy(const m3d::AIParam& PParam);
        void ConvertFromString(void* retVal, m3d::eAIParamType ToType) const;
        static int __cdecl CompareInt(const void* v1, const void* v2);
        static int __cdecl CompareStr(const void* v1, const void* v2);

        struct RepEl
        {
            /* 0x0000 */ CStr m_prototypeName;
            /* 0x000c */ int m_Amount;
            bool operator==(const m3d::AIParam::RepEl&);
            RepEl(const m3d::AIParam::RepEl&);
            RepEl(const CStr&, const int&);
        }; /* size: 0x0010 */

    protected:
        static int __fastcall CompareRepEl(const void*, const void*);

    public:
        /* 0x0014 */ CStr(*NameFromNum)(const m3d::AIParam*, int);
        /* 0x0018 */ int (*NumFromName)(const m3d::AIParam*, CStr&);
        void ReadFromString(const CStr& Str);
        void Clear();
        ~AIParam();
        AIParam(const float& PValue);
        AIParam(const Quaternion&);
        AIParam(const CVector& pos);
        AIParam(const CVector2& range);
        AIParam(const int& PID);
        AIParam(const unsigned int&);
        AIParam(const CStr& str);
        AIParam();
        AIParam(const m3d::AIParam& PParam);
        AIParam(const oldstd::vector<int>& List);
        bool operator>(const m3d::AIParam& with);
        bool operator<(const m3d::AIParam& with);
        bool operator>=(const m3d::AIParam&);
        bool operator<=(const m3d::AIParam& with);
        bool operator==(const m3d::AIParam& with);
        bool operator!=(const m3d::AIParam& with);
        void Init();
        m3d::AIParam& operator=(const float&);
        m3d::AIParam& operator=(const Quaternion&);
        m3d::AIParam& operator=(const CVector&);
        m3d::AIParam& operator=(const CVector2&);
        m3d::AIParam& operator=(const int&);
        m3d::AIParam& operator=(const unsigned int& PID);
        m3d::AIParam& operator=(const CStr&);
        m3d::AIParam& operator=(const oldstd::vector<CStr>&);
        m3d::AIParam& operator=(const oldstd::vector<int>&);
        CVector GetAsVector() const;
        Quaternion GetAsQuaternion() const;
        CVector2 GetAsRange() const;
        int GetAsID() const;
        float GetAsFloat() const;
        oldstd::vector<int> GetAsIdList() const;
        oldstd::vector<CStr> GetAsStringList() const;
        void SetType(m3d::eAIParamType ParamType);
        m3d::eAIParamType GetType() const;
        CStr ToStr() const;
        CStr GetAsStr() const;
        void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* OwnNode);
        void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const;
    }; /* size: 0x001c */
}
